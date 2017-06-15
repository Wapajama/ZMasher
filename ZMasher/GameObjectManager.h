#pragma once
#include "TransformComponentManager.h"
#include "MeshComponentManager.h"
#include "BulletComponentManager.h"
#include "BulletSystem.h"
#include "AIComponentManager.h"
#include <ZMasher\GameObject.h>
#include <ZMasher\CollisionComponentManager.h>
#include <ZMasher\CollisionSystem.h>
#include <ZMasher\AISystem.h>
#include <Time/Profiler.h>

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
	inline AIComponentManager* AICompManager() {return &m_AICompManager;}

private:

	void UpdateAllComponentManagers();

	GrowArray <ComponentManager*> m_ComponentManagers;

	ID_TYPE m_CurrentID;

	//TODO: GameObjectManager is getting blobbed because of all the componentmanagers and systems, extract to another class
	GameObject m_GameObjects[NUMBER_OF_GAME_OBJECTS];

	MeshComponentManager m_MeshManager;
	TransformComponentManager m_TransformManager;
	BulletComponentManager m_BulletCompManager;
	AIComponentManager m_AICompManager;

	BulletSystem m_BulletSystem;
	CollisionSystem m_CollisionSystem;
	CollisionComponentManager m_CollisionCompManager;
	AISystem m_AISystem;

#ifdef BENCHMARK
	ProfilerTaskID m_CollisionTimeStamp;
	ProfilerTaskID m_AITimeStamp;
	ProfilerTaskID m_MeshCompManagerTimeStamp;
#endif // BENCHMARK


};
