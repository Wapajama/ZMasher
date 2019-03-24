#include "CameraState.h"
#include "project_defines.h"
#include <ZMasher\InputManager.h>
#include <Math\ZMVector3.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef ZMASHER_DX11
#include <ZMasherGfxDX11/Camera.h>
#elif defined(ZMASHER_DX12)
#include <ZMasherGfxDX12/Camera.h>
#else
#error No renderer specified!
#endif

CameraState::CameraState() :
	m_SpeedModifier(1.f)
{
	m_CameraForwardMatrix = ZMasher::Matrix44f::Identity();
}

CameraState::~CameraState()
{
}

bool CameraState::Init(const char* args)
{
	m_Camera->SetPosition(ZMasher::Vector3f(0, 10, 0));
	return true;
}

bool CameraState::Update(const float dt)
{
	m_Dt = dt;
	const float prev_modifier = m_SpeedModifier;
#ifndef BENCHMARK
	Movement();
	MouseRotation();
#endif // !BENCHMARK


	m_SpeedModifier = prev_modifier;
	return true;
}
#define KEY_DOWN(key) InputManager::Instance()->IsKeyDown(key)
void CameraState::Movement()
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

const float global_rotation_speed = 0.01f;
void CameraState::MouseRotation()
{


	m_PrevMousePos = m_MousePos;
	m_MousePos = InputManager::Instance()->MousePos();

	if( !InputManager::Instance()->IsMouseDown(DIK_LMOUSE))
	{
		return;
	}

	const ZMasher::Vector2i diff_pos = m_PrevMousePos - m_MousePos;

	ZMasher::Matrix44f cam_orientation = m_Camera->GetWorldOrientation();
	const ZMasher::Vector4f vector_up = cam_orientation.GetVectorUp();
	const ZMasher::Vector4f vector_lf = cam_orientation.GetVectorLeft();
	const ZMasher::Vector4f cam_trans = cam_orientation.GetTranslation();

	cam_orientation.SetTranslation(ZMasher::Vector4f(0, 0, 0, cam_trans.w));

	// Aligning the forward matrix with the camera matrix

	cam_orientation *= ZMasher::Matrix44f::CreateRotationMatrixAroundAxis(vector_lf, global_rotation_speed*diff_pos.y);

	// Means that the camera has rotated further than "straight up" or "straight down". 
	const ZMasher::Vector4f cam_forw = cam_orientation.GetVectorForward();
	const ZMasher::Vector4f fowcam_forw = m_CameraForwardMatrix.GetVectorForward();
	const float rotation_result = ZMasher::Dot(cam_orientation.GetVectorForward(), m_CameraForwardMatrix.GetVectorForward());
	if ( rotation_result < 0.f)
	{
		cam_orientation *= ZMasher::Matrix44f::CreateRotationMatrixAroundAxis(vector_lf, -(global_rotation_speed*diff_pos.y));
	}

	cam_orientation.SetTranslation(cam_trans);

	cam_orientation.m_Vectors[0].Normalize();
	cam_orientation.m_Vectors[1].Normalize();
	cam_orientation.m_Vectors[2].Normalize();

	m_Camera->SetOrientation(cam_orientation);

	m_Camera->RotateY(global_rotation_speed*diff_pos.x);
	m_CameraForwardMatrix.RotateY(global_rotation_speed*diff_pos.x);
}

const float global_speed = 200.f;

void CameraState::MoveForward()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorForward()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void CameraState::MoveBackwards()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorForward()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void CameraState::MoveRight()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorLeft()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void CameraState::MoveLeft()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorLeft()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void CameraState::MoveUp()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() + ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorUp()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void CameraState::MoveDown()
{
	ZMasher::Vector3f translation = m_Camera->GetPosition() - ZMasher::Vector3f(m_Camera->GetWorldOrientation().GetVectorUp()*global_speed*m_SpeedModifier*m_Dt);
	m_Camera->SetPosition(translation);
}

void CameraState::SetCamera(Camera* camera)
{
	m_Camera = camera;

	m_CameraForwardMatrix.RotateX(m_Camera->GetWorldOrientation().GetRotationY());
}