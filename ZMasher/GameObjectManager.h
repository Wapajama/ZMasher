#pragma once
#include "TransformComponentManager.h"
#include "MeshComponentManager.h"
//Dataoriented entitysystem
#include <ZMasher\GameObject.h>

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Update(const float dt);

	bool Init();
	void Destroy();

	GameObject CreateGameObject();

	bool IsAlive(GameObject game_object);
	void Destroy(GameObject& game_Object);

	inline MeshComponentManager* MeshCompManager() { return &m_MeshManager; }
	inline TransformComponentManager* TransformManager() { return &m_TransformManager; }
private:
	ID_TYPE m_CurrentID;

	TransformComponentManager m_TransformManager;
	MeshComponentManager m_MeshManager;
	GameObject m_GameObjects[NUMBER_OF_GAME_OBJECTS];

};
