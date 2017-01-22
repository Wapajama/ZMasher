#pragma once
#include "GameState.h"
#include <DataStructures\GrowArray.h>
#include <ZMasher\GameObjectManager.h>

class GameplayState :
	public GameState
{
public:
	GameplayState();
	~GameplayState();

	bool Init(const char* args)override;
	bool Update(const float dt)override;
	
private:
	GameObjectManager m_GameObjectManager;
	GrowArray<GameObject> m_GameObjects;
};
