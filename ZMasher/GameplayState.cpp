#include "GameplayState.h"
#include <ZMasher\ZMModelFactory.h>
#include <ZMasher\InputManager.h>
#include <ZMasher\Camera.h>
#include <Math\ZMVector3.h>
#define _USE_MATH_DEFINES
#include <math.h>

GameplayState::GameplayState(Camera* camera) 
	: m_Camera(camera),
	m_RotationX(0), 
	m_RotationY(0)
{
}


GameplayState::~GameplayState()
{
}

bool GameplayState::Init(const char* args)
{
	ZMasher::Vector4f position(0, 1, 0.f, 1.f);
	ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	for (int i = 0; i < 10; ++i)
	{
		GameObject new_object = m_GameObjectManager.CreateGameObject();
		m_GameObjects.Add(new_object);
		transform.SetTranslation(position + ZMasher::Vector4f(0, 0, -500 + i * 100, 0));
		if (i%2)
		{
			transform.RotateY(M_PI / 2);
		}
		m_GameObjectManager.TransformManager()->AddComponent(new_object, transform);
		m_GameObjectManager.MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance("../data/Truax_Studio_Mac11.FBX"));
	}

	return true;
}

bool GameplayState::Update(const float dt)
{
#define KEY_DOWN(key) InputManager::Instance()->IsKeyDown(key)
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
	if (KEY_DOWN(DIKEYBOARD_LEFT))
	{
		RotateLeft();
	}
	if (KEY_DOWN(DIKEYBOARD_RIGHT))
	{
		RotateRight();
	}
	if (KEY_DOWN(DIKEYBOARD_UP))
	{
		RotateUp();
	}
	if (KEY_DOWN(DIKEYBOARD_DOWN))
	{
		RotateDown();
	}

	MouseRotation(dt);

	m_GameObjectManager.Update(dt);

	return true;
}

const float global_speed = -200.f;

void GameplayState::MoveForward()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + ZMasher::Vector3f(0,0, -global_speed * 0.016f));
}

void GameplayState::MoveBackwards()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + ZMasher::Vector3f(0,0, global_speed * 0.016f));
}

void GameplayState::MoveRight()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + ZMasher::Vector3f(-global_speed * 0.016f, 0,0));
}

void GameplayState::MoveLeft()
{
	m_Camera->SetPosition(m_Camera->GetPosition() + ZMasher::Vector3f(global_speed * 0.016f, 0,0));
}

const float global_rotation_speed = 0.5f;

void GameplayState::RotateRight()
{
	m_Camera->RotateY(global_rotation_speed);
	m_RotationY += global_rotation_speed;
}

void GameplayState::RotateLeft()
{
	m_Camera->RotateY(-global_rotation_speed);
	m_RotationY -= global_rotation_speed;
}

void GameplayState::RotateUp()
{
	m_RotationX -= global_rotation_speed;
}

void GameplayState::RotateDown()
{
	m_RotationX += global_rotation_speed;
}
static int derp = 0;
void GameplayState::MouseRotation(const float dt)
{
	const ZMasher::Vector2i mouse_pos = InputManager::Instance()->MousePos();
	const ZMasher::Vector2i diff_pos = m_PrevMousePos - mouse_pos;
	derp++;
	//m_PrevMousePos = mouse_pos;
	m_Camera->RotateX(global_rotation_speed*diff_pos.y*dt);
	m_Camera->RotateY(global_rotation_speed*diff_pos.x*dt);
}