#pragma once
#include "GameState.h"
#include <DataStructures\GrowArray.h>
#include <Math\Vector2.h>
#include <Math\ZMMatrix44.h>
class Camera;

class GameplayState :
	public GameState
{
public:
	GameplayState(Camera* camera);
	~GameplayState();

	bool Init(const char* args)override;
	bool Update(const float dt)override;
	
private:
	//lazy implementation
	ZMasher::Vector2i m_PrevMousePos;
	ZMasher::Vector2i m_MousePos;

	void Movement();

	void MouseRotation(const float dt);

	void MoveForward();
	void MoveBackwards();
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	void ShootBullet();
	void SpawnEnemy();

	Camera* m_Camera;
	ZMasher::Matrix44f m_CameraForwardMatrix;
	float m_Dt;
	float m_RotationX;
	float m_RotationY;
	float m_SpeedModifier;
};