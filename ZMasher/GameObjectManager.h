#pragma once
#include "TransformComponentManager.h"
#include "MeshComponentManager.h"
#include "BulletComponentManager.h"
#include "BulletSystem.h"
#include <ZMasher\GameObject.h>
#include <ZMasher\CollisionComponentManager.h>
#include <ZMasher\CollisionSystem.h>

//Dataoriented entitysystem
class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Update(const float dt);
	bool Init();
	void Destroy();

	GameObject CreateGameObject();

	void Destroy(GameObject& game_Object);

	inline MeshComponentManager* MeshCompManager() { return &m_MeshManager; }
	inline TransformComponentManager* TransformManager() { return &m_TransformManager; }
	inline BulletComponentManager* BulletCompManager() {return &m_BulletCompManager;}
	inline CollisionComponentManager* CollisionCompManager() {return &m_CollisionCompManager;}
private:

	void UpdateAllComponentManagers();

	GrowArray <ComponentManager*> m_ComponentManagers;

	ID_TYPE m_CurrentID;

	MeshComponentManager m_MeshManager;//TODO: This class is getting blobbed because of all the componentmanagers, extract to another class
	TransformComponentManager m_TransformManager;
	BulletComponentManager m_BulletCompManager;
	GameObject m_GameObjects[NUMBER_OF_GAME_OBJECTS];
	BulletSystem m_BulletSystem;//Holy mother of jesus christ wtf is this doing here EXTRACT ASAP
	CollisionSystem m_CollisionSystem;
	CollisionComponentManager m_CollisionCompManager;
};
