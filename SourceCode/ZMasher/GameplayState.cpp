
#include "GameplayState.h"
#ifdef ZMASHER_DX11
#include <ZMasherGfxDX11/Camera.h>
#elif defined(ZMASHER_DX12)
#include <ZMasherGfxDX12/Camera.h>
#else
#error No renderer specified!
#endif
#include <ZMasherGfxDX11\ZMModelFactory.h>
#include <ZMasher\InputManager.h>
#include <Math\ZMVector3.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>
#include <DataStructures\BinarySearchTree.h>
#include <ZMasher\GameObjectManager.h>
#include <ZMUtils\File\PathManager.h>
#define _USE_MATH_DEFINES
#include <math.h>

GameplayState::GameplayState(Camera* camera)
	: m_Camera(camera),
	m_RotationX(0),
	m_RotationY(0),
	m_SpeedModifier(1.f)
{
	m_CameraForwardMatrix.RotateX(m_Camera->GetWorldOrientation().GetRotationY());
}

GameplayState::~GameplayState()
{
}

void(*g_TestCallBack)(CollCallbackArgs) = [](CollCallbackArgs args)
{

	if (!args.b)
	{
		if (args.a->m_CollisionFilter == eCOLLISIONTYPE::eTurretBullet)
		{
			GameObjectManager::Instance()->Destroy(args.a->m_GameObject);
		}
		return;
	}

	if (args.a->m_CollisionFilter == eCOLLISIONTYPE::eEnemy &&
		args.b->m_CollisionFilter == eCOLLISIONTYPE::eEnemy)
	{
		return;
	}

	if (args.a->m_CollisionFilter == eCOLLISIONTYPE::eTurretBullet &&
		args.b->m_CollisionFilter == eCOLLISIONTYPE::eTurretBullet)
	{
		return;
	}

	if (args.a->m_CollisionFilter == eCOLLISIONTYPE::eTurretBullet)
	{
		GameObjectManager::Instance()->Destroy(args.a->m_GameObject);
		if (args.b->m_CollisionFilter == eCOLLISIONTYPE::eEnemy)
		{
			GameObjectManager::Instance()->Destroy(args.b->m_GameObject);
		}
	}
	else if (args.b->m_CollisionFilter == eCOLLISIONTYPE::eTurretBullet)
	{
		GameObjectManager::Instance()->Destroy(args.b->m_GameObject);
		if (args.a->m_CollisionFilter == eCOLLISIONTYPE::eEnemy)
		{
			GameObjectManager::Instance()->Destroy(args.a->m_GameObject);
		}
	}
};

#ifndef _DEBUG
#define NUMBER_OF_ENEMIES 300
#define NUMBER_OF_BOXES 30
#else
#define NUMBER_OF_ENEMIES 200
#define NUMBER_OF_BOXES 10
#endif // !_DEBUG

const float ai_range = AI_RANGE;
bool GameplayState::Init(const char* args)
{
	GameObjectManager::Create();
	

	ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	transform.SetTranslation(ZMasher::Vector4f(ZMasher::Vector3f(0, 1, 0), 1.f));
	//transform.RotateY(M_PI_2);

	GameObject new_object = GameObjectManager::Instance()->CreateGameObject();
	GameObjectManager::Instance()->TransformManager()->AddComponent(new_object, transform);
	GameObjectManager::Instance()->MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance((PathManager::Instance()->GetDataPath() + "dragonfly01/dragonfly01.model").c_str()));
	GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 5, new_object, g_TestCallBack);
	GameObjectManager::Instance()->MomentumCompManager()->AddComponent(new_object, 4);
	GameObjectManager::Instance()->AICompManager()->AddComponent(new_object, eAIType::BASIC_TURRET);
	GameObjectManager::Instance()->GetCollisionSystem()->CreateQuery(eCOLLISIONTYPE::eTurret, new_object, 300, transform.GetTranslation().ToVector3f());
	SpawnEnemy(true);

	for (int i = 0; i < NUMBER_OF_BOXES; ++i)
	{
		SpawnAABB();
	}

	return true;
}

void GameplayState::SpawnEnemy(bool random_x)
{
	AIObjectArgs args;
	args.targetPos = ZMasher::Vector3f(0, 1, 0);
	args.modelName = PathManager::Instance()->GetDataPath() + "dragonfly01/dragonfly01.model";
	args.aiType = eAIType::ZOLDIER;
	args.radius = 5;
	args.speed = 4;
	args.collisionType = eCOLLISIONTYPE::eEnemy;

	AIGroup* g = GameObjectManager::Instance()->GetAISystem()->CreateAIs(&args, NUMBER_OF_ENEMIES);
	// m_Camera->SetPosition(ZMasher::Vector3f(g->GetCurrentObject()->tf->m_Transform.GetTranslation().x, 75, ai_range * 0.5f - 150g->GetCurrentObject()->tf->m_Transform.GetTranslation().x));
	// m_Camera->SetPosition(ZMasher::Vector3f(g->GetCurrentObject()->tf->m_Transform.GetTranslation().x, 75, g->GetCurrentObject()->tf->m_Transform.GetTranslation().z));
}

void GameplayState::SpawnAABB()
{
	float halfWidth[3] = { 10,5,10 };
	ZMasher::Vector4f position(0, 1, halfWidth[1], 1.f);
	ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	transform.SetTranslation(position + ZMasher::Vector4f(ZMasher::GetRandomFloat(-ai_range, ai_range), position.y, ZMasher::GetRandomFloat(-ai_range, ai_range), 0));

	//transform.SetTranslation(ZMasher::Vector4f( 500, position.y, 15, 0));

	GameObject new_object = GameObjectManager::Instance()->CreateGameObject();
	GameObjectManager::Instance()->TransformManager()->AddComponent(new_object, transform);
	GameObjectManager::Instance()->AABBCompManager()->AddComponent(eCOLLISIONTYPE::eBlocker, halfWidth, new_object, g_TestCallBack);
}

#define KEY_DOWN(key) InputManager::Instance()->IsKeyDown(key)

bool GameplayState::Update(const float dt)
{
	m_Dt = dt;
	const float prev_modifier = m_SpeedModifier;
	Movement();

	m_SpeedModifier = prev_modifier;

#define GUN_COOLDOWN 0.1
#ifndef BENCHMARK
	static float lazy_timer = GUN_COOLDOWN;
	lazy_timer -= dt;
	if ((InputManager::Instance()->IsMouseDown(DIK_LMOUSE) &&
		 lazy_timer <= 0.f))
	{
		if (lazy_timer < GUN_COOLDOWN)
		{
			//ShootBullet();
			lazy_timer = GUN_COOLDOWN;
		}
	}
	MouseRotation(dt);
#else
	static float lazy_timer = GUN_COOLDOWN;
	lazy_timer -= dt;
	if (lazy_timer <= 0.f)
	{
		if (lazy_timer < GUN_COOLDOWN)
		{
			//ShootBullet();
			lazy_timer = GUN_COOLDOWN;
		}
	}
#endif // BENCHMARK

	if (GameObjectManager::Instance()->AICompManager()->GetNumberOfAIs() < (NUMBER_OF_ENEMIES / 2))
	{
		//const short n_ais = GameObjectManager::Instance()->AICompManager()->GetNumberOfAIs();
		//for (int i = 0; i < NUMBER_OF_ENEMIES - n_ais; i++)
		//{
		SpawnEnemy();
		//}
	}

	GameObjectManager::Instance()->Update(dt);

	return true;
}

bool GameplayState::Destroy()
{
	GameObjectManager::Instance()->Destroy();
	return true;
}

const float global_speed = 200.f;

void GameplayState::MoveForward()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorForward() * global_speed * m_SpeedModifier * m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveBackwards()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorForward() * global_speed * m_SpeedModifier * m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveRight()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorLeft() * global_speed * m_SpeedModifier * m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveLeft()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorLeft() * global_speed * m_SpeedModifier * m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveUp()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorUp() * global_speed * m_SpeedModifier * m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveDown()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorUp() * global_speed * m_SpeedModifier * m_Dt);
	m_Camera->SetPosition(translation);
}

const float global_bullet_speed = 300.f;
void GameplayState::ShootBullet()
{
	GameObject bullet = GameObjectManager::Instance()->CreateGameObject();
	ZMasher::Matrix44f bulletTransform = m_Camera->GetWorldOrientation();
	bulletTransform.SetTranslation(ZMasher::Vector4f(m_Camera->GetPosition(), 1));
	bulletTransform.SetTranslation(bulletTransform.GetTranslation() + m_Camera->GetWorldOrientation().GetVectorForward() * 20);
	GameObjectManager::Instance()->TransformManager()->AddComponent(bullet, bulletTransform);
	GameObjectManager::Instance()->MeshCompManager()->AddComponent(bullet, ZMModelFactory::Instance()->LoadModelInstance((PathManager::Instance()->GetDataPath() + "sphere.model").c_str()));
	GameObjectManager::Instance()->BulletCompManager()->AddComponent(bullet, 1.f, 1337, 3);
	GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 20, bullet, g_TestCallBack);
	GameObjectManager::Instance()->MomentumCompManager()->AddComponent(bullet, 10, (m_Camera->GetWorldOrientation().GetVectorForward()).ToVector3f() * global_bullet_speed);
}

const float global_rotation_speed = 0.01f;

void GameplayState::MouseRotation(const float dt)
{
	m_PrevMousePos = m_MousePos;
	m_MousePos = InputManager::Instance()->MousePos();
	const ZMasher::Vector2i diff_pos = m_PrevMousePos - m_MousePos;

	ZMasher::Matrix44f cam_orientation = m_Camera->GetWorldOrientation();
	const ZMasher::Vector4f vector_up = cam_orientation.GetVectorUp();
	const ZMasher::Vector4f vector_lf = cam_orientation.GetVectorLeft();
	const ZMasher::Vector4f cam_trans = cam_orientation.GetTranslation();

	cam_orientation.SetTranslation(ZMasher::Vector4f(0, 0, 0, cam_trans.w));

	// Aligning the forward matrix with the camera matrix

	cam_orientation *= ZMasher::Matrix44f::CreateRotationMatrixAroundAxis(vector_lf, global_rotation_speed * diff_pos.y);

	// Means that the camera has rotated further than "straight up" or "straight down". 
	const ZMasher::Vector4f cam_forw = cam_orientation.GetVectorForward();
	const ZMasher::Vector4f fowcam_forw = m_CameraForwardMatrix.GetVectorForward();
	const float rotation_result = ZMasher::Dot(cam_orientation.GetVectorForward(), m_CameraForwardMatrix.GetVectorForward());
	if (rotation_result < 0.f)
	{
		cam_orientation *= ZMasher::Matrix44f::CreateRotationMatrixAroundAxis(vector_lf, -(global_rotation_speed * diff_pos.y));
	}

	cam_orientation.SetTranslation(cam_trans);

	cam_orientation.m_Vectors[0].Normalize();
	cam_orientation.m_Vectors[1].Normalize();
	cam_orientation.m_Vectors[2].Normalize();

	m_CameraForwardMatrix.m_Vectors[0].Normalize();
	m_CameraForwardMatrix.m_Vectors[1].Normalize();
	m_CameraForwardMatrix.m_Vectors[2].Normalize();

	m_Camera->SetOrientation(cam_orientation);

	m_Camera->RotateY(global_rotation_speed * diff_pos.x);
	m_CameraForwardMatrix.RotateY(global_rotation_speed * diff_pos.x);

}

void GameplayState::Movement()
{
	if (KEY_DOWN(DIKEYBOARD_LCONTROL))
	{
		m_SpeedModifier = 0.1f;
	}
	if (KEY_DOWN(DIKEYBOARD_W))
	{
		MoveForward();
	}
	if (KEY_DOWN(DIKEYBOARD_S))
	{
		MoveBackwards();
	}
	if (KEY_DOWN(DIKEYBOARD_A))
	{
		MoveLeft();
	}
	if (KEY_DOWN(DIKEYBOARD_D))
	{
		MoveRight();
	}
	if (KEY_DOWN(DIKEYBOARD_R))
	{
		MoveUp();
	}
	if (KEY_DOWN(DIKEYBOARD_F))
	{
		MoveDown();
	}
}