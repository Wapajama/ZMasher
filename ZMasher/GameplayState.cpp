#include "GameplayState.h"
#include <ZMasherGfxDX11\ZMModelFactory.h>
#include <ZMasher\InputManager.h>
#include <ZMasher\Camera.h>
#include <Math\ZMVector3.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <DataStructures\BinarySearchTree.h>

GameplayState::GameplayState(Camera* camera) 
	: m_Camera(camera),
	m_RotationX(0), 
	m_RotationY(0),
	m_SpeedModifier(1.f)
{
}

GameplayState::~GameplayState()
{
}

bool GameplayState::Init(const char* args)
{
	m_Camera->SetPosition(ZMasher::Vector3f(100, 40, -100));

	ZMasher::Vector4f position(0, 1, 0.f, 1.f);
	ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	m_GameObjectManager.Init();
	const float range = 100;
	for (int i = 0; i < 500; ++i)
	{
		transform.SetTranslation(position + ZMasher::Vector4f(ZMasher::GetRandomFloat(-range, range), 0, ZMasher::GetRandomFloat(-range, range), 0));
		if (i%2)
		{
			transform.RotateY(M_PI / 2);
		}
		GameObject new_object = m_GameObjectManager.CreateGameObject();
		m_GameObjects.Add(new_object);
		m_GameObjectManager.TransformManager()->AddComponent(new_object, transform);
		m_GameObjectManager.MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance("../data/dragonfly01/dragonfly01.model"));
		m_GameObjectManager.CollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 15, new_object, 10);
		m_GameObjectManager.AICompManager()->AddComponent(new_object, eAIType::ZOLDIER);
		
		m_GameObjectManager.AICompManager()->GetComponent(new_object)->m_TargetPos = (ZMasher::Vector3f(100, 1, transform.GetTranslation().z));
	}

	return true;
}

bool GameplayState::Update(const float dt)
{
	m_Dt = dt;
#define KEY_DOWN(key) InputManager::Instance()->IsKeyDown(key)
	const float prev_modifier = m_SpeedModifier;
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
	m_SpeedModifier = prev_modifier;
	static float lazy_timer = 0.f;
	lazy_timer -= dt;
	if (InputManager::Instance()->IsMouseDown(DIK_LMOUSE) &&
		lazy_timer <= 0.f)
	{
		//SPAWN BULLET FROM PLAYER
		lazy_timer = 1.01;
		//ShootBullet();
	}
	MouseRotation(dt);

	m_GameObjectManager.Update(dt);

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
	GameObject bullet = m_GameObjectManager.CreateGameObject();
	m_GameObjectManager.TransformManager()->AddComponent(bullet, m_Camera->GetWorldOrientation());
	m_GameObjectManager.MeshCompManager()->AddComponent(bullet, ZMModelFactory::Instance()->LoadModelInstance("../data/sphere.model"));
	m_GameObjectManager.BulletCompManager()->AddComponent(bullet, 10.f, 1337, 3);
	m_GameObjectManager.CollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 15, bullet, 10);
}

const float global_rotation_speed = 0.2f;

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

	cam_orientation *= ZMasher::Matrix44f::CreateRotationMatrixAroundAxis(vector_lf, global_rotation_speed*diff_pos.y*dt);

	cam_orientation.SetTranslation(cam_trans);

	m_Camera->SetOrientation(cam_orientation);

	m_Camera->RotateY(global_rotation_speed*diff_pos.x*dt);
}