#include "GameplayState.h"
#include <ZMasherGfxDX11\ZMModelFactory.h>
#include <ZMasher\InputManager.h>
#include <ZMasher\Camera.h>
#include <Math\ZMVector3.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>
#include <DataStructures\BinarySearchTree.h>
#include <ZMasher\GameObjectManager.h>
#define _USE_MATH_DEFINES
#include <math.h>

GameplayState::GameplayState(Camera* camera) 
	: m_Camera(camera),
	m_RotationX(0), 
	m_RotationY(0),
	m_SpeedModifier(1.f)
{
	ZMModelInstanceNode* enemy = ZMModelFactory::Instance()->LoadModelInstance("../../data/dragonfly01/dragonfly01.model");
	ZMModelInstanceNode* bullet = ZMModelFactory::Instance()->LoadModelInstance("../../data/sphere.model");
	enemy->MarkForDelete();
	bullet->MarkForDelete();
}

GameplayState::~GameplayState()
{
}


void(*g_TestCallBack)(CollCallbackArgs) = [](CollCallbackArgs args) 
{
	GameObjectManager::Instance()->Destroy(args.a->m_GameObject);
	GameObjectManager::Instance()->Destroy(args.b->m_GameObject);
	return; 
};

#define NUMBER_OF_ENEMIES 10

bool GameplayState::Init(const char* args)
{	
	GameObjectManager::Create();
	m_Camera->SetPosition(ZMasher::Vector3f(100, 0, -100));
	
	for (int i = 0; i < NUMBER_OF_ENEMIES; ++i)
	{
		SpawnEnemy();
	}

	return true;
}

void GameplayState::SpawnEnemy()
{
	const float range = 100;
	ZMasher::Vector4f position(0, 1, 0.f, 1.f);
	ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	transform.SetTranslation(position + ZMasher::Vector4f(ZMasher::GetRandomFloat(-range, range), 0, ZMasher::GetRandomFloat(-range, range), 0));
	//if (i % 2)
	//{
	//	transform.RotateY(M_PI / 2);
	//}
	GameObject new_object = GameObjectManager::Instance()->CreateGameObject();
	GameObjectManager::Instance()->TransformManager()->AddComponent(new_object, transform);
	GameObjectManager::Instance()->MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance("../../data/dragonfly01/dragonfly01.model"));
	GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 0.1, new_object, g_TestCallBack);
	GameObjectManager::Instance()->MomentumCompManager()->AddComponent(new_object, 10);
	
	GameObjectManager::Instance()->AICompManager()->AddComponent(new_object, eAIType::ZOLDIER);

	GameObjectManager::Instance()->AICompManager()->GetComponent(new_object)->m_TargetPos = (ZMasher::Vector3f(100, 1, transform.GetTranslation().z));
}

#define KEY_DOWN(key) InputManager::Instance()->IsKeyDown(key)
bool GameplayState::Update(const float dt)
{
	m_Dt = dt;
	const float prev_modifier = m_SpeedModifier;
	//Movement();

	m_SpeedModifier = prev_modifier;

#define GUN_COOLDOWN 0.1
//#ifndef BENCHMARK
//	static float lazy_timer = GUN_COOLDOWN;
//	lazy_timer -= dt;
//	if ((InputManager::Instance()->IsMouseDown(DIK_LMOUSE) &&
//		lazy_timer <= 0.f))
//	{
//		if(lazy_timer < GUN_COOLDOWN)
//		{
//			ShootBullet();
//			lazy_timer = GUN_COOLDOWN;
//		}
//	}
//	//MouseRotation(dt);
//#else
	static float lazy_timer = GUN_COOLDOWN;
	lazy_timer -= dt;
	if (lazy_timer <= 0.f)
	{
		if(lazy_timer < GUN_COOLDOWN)
		{
			ShootBullet();
			lazy_timer = GUN_COOLDOWN;
		}
	}
//#endif // PROFILE

	if (GameObjectManager::Instance()->AICompManager()->GetNumberOfAIs() < NUMBER_OF_ENEMIES)
	{
		const short n_ais = GameObjectManager::Instance()->AICompManager()->GetNumberOfAIs();
		for (int i = 0; i < NUMBER_OF_ENEMIES - n_ais; i++)
		{
			SpawnEnemy();
		}
	}

	GameObjectManager::Instance()->Update(dt);

	return true;
}

const float global_speed = 200.f;

void GameplayState::MoveForward()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorForward()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveBackwards()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorForward()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveRight()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorLeft()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveLeft()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorLeft()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveUp()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorUp()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::MoveDown()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorUp()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void GameplayState::ShootBullet()
{
	GameObject bullet = GameObjectManager::Instance()->CreateGameObject();
	ZMasher::Matrix44f bulletTransform = m_Camera->GetWorldOrientation();
	bulletTransform.SetTranslation(bulletTransform.GetTranslation() + m_Camera->GetWorldOrientation().GetVectorForward() * 20);
	GameObjectManager::Instance()->TransformManager()->AddComponent(bullet, bulletTransform);
	GameObjectManager::Instance()->MeshCompManager()->AddComponent(bullet, ZMModelFactory::Instance()->LoadModelInstance("../../data/sphere.model"));
	GameObjectManager::Instance()->BulletCompManager()->AddComponent(bullet, 30.f, 1337, 3);
	GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 15, bullet, g_TestCallBack);
	GameObjectManager::Instance()->MomentumCompManager()->AddComponent(bullet, 10, (m_Camera->GetWorldOrientation().GetVectorForward()).ToVector3f() * 3000.f);
}

const float global_rotation_speed = 1.f;

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

	cam_orientation *= ZMasher::Matrix44f::CreateRotationMatrixAroundAxis(vector_lf, global_rotation_speed*dt*diff_pos.y);

	cam_orientation.SetTranslation(cam_trans);

	m_Camera->SetOrientation(cam_orientation);

	m_Camera->RotateY(global_rotation_speed*diff_pos.x*dt);
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