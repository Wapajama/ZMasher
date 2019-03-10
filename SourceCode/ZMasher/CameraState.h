#pragma once
#include "GameState.h"
#include <Math\Vector2.h>
#include <Math\ZMMatrix44.h>


class Camera;

class CameraState :
	public GameState
{
public:
	CameraState();
	~CameraState();

	bool Init(const char* args)override;
	bool Update(const float dt)override;
	bool Destroy()override { return true; }
	
	inline Camera* GetCamera(){return m_Camera;}	
	void SetCamera(Camera* camera);

private:
	
	ZMasher::Vector2i m_PrevMousePos;
	ZMasher::Vector2i m_MousePos;

	Camera* m_Camera;

	void Movement();

	void MouseRotation();

	void MoveForward();
	void MoveBackwards();
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	ZMasher::Matrix44f m_CameraForwardMatrix;
	float m_Dt;
	float m_RotationX;
	float m_RotationY;
	float m_SpeedModifier;
};

