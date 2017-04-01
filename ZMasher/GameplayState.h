#pragma once
#include "GameState.h"
#include <DataStructures\GrowArray.h>
#include <ZMasher\GameObjectManager.h>
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


	void MouseRotation(const float dt);

	void MoveForward();
	void MoveBackwards();
	void MoveRight();
	void MoveLeft();

	void ShootBullet();

	GameObjectManager m_GameObjectManager;
	GrowArray<GameObject> m_GameObjects;
	Camera* m_Camera;
	ZMasher::Matrix44f m_TransToFollow;
	float m_Dt;
	float m_RotationX;
	float m_RotationY;

};
