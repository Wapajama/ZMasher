#pragma once
#include "TransformComponentManager.h"
#include "MeshComponentManager.h"
#include "BulletComponentManager.h"
#include "BulletSystem.h"
#include "AIComponentManager.h"
#include <ZMasher\GameObject.h>
#include <ZMasher\SphereCollisionComponentManager.h>
#include <ZMasher\AABBComponentManager.h>
#include <ZMasher\MomentumComponentManager.h>
#include <ZMasher/DebugLineComponentManager.h>
#include <ZMasher\CollisionSystem.h>
#include <ZMasher\AISystem.h>
#include <Time/Timer.h>
#include <Utility\ZMSingleton.h>

//Dataoriented entitysystem
class GameObjectManager
	: public ZMSingleton<GameObjectManager>
{
public:
	void Update(const float dt);
	bool Init();
	void Destroy();

	GameObject CreateGameObject();
	bool Alive(GameObject game_object);
	void Destroy(GameObject game_object, bool remove_everywhere = true);

	inline MeshComponentManager* MeshCompManager() { return &m_MeshManager; }
	inline TransformComponentManager* TransformManager() { return &m_TransformManager; }
	inline BulletComponentManager* BulletCompManager() {return &m_BulletCompManager;}
	inline SphereCollisionComponentManager* SphereCollisionCompManager() {return &m_SphereCollisionCompManager;}
	inline MomentumComponentManager* MomentumCompManager() { return &m_MomentumComponentManager; }
	inline AIComponentManager* AICompManager() {return &m_AICompManager;}
	inline AABBComponentManager* AABBCompManager() {return &m_AABBComponentManager;}
	inline CollisionSystem* GetCollisionSystem() {return &m_CollisionSystem;}
	inline AISystem* GetAISystem() { return &m_AISystem; }

protected:

private:
	GameObjectManager();
	~GameObjectManager();
	friend class ZMSingleton<GameObjectManager>;

	void UpdateAllComponentManagers();

	GrowArray <IComponentManager*> m_ComponentManagers;

	GrowArray<GO_ID_TYPE> m_FreeIndexes;

	MeshComponentManager m_MeshManager;
	TransformComponentManager m_TransformManager;
	BulletComponentManager m_BulletCompManager;
	AIComponentManager m_AICompManager;
	BulletSystem m_BulletSystem;
	SphereCollisionComponentManager m_SphereCollisionCompManager;
	AABBComponentManager m_AABBComponentManager;
	MomentumComponentManager m_MomentumComponentManager;

	CollisionSystem m_CollisionSystem;
	AISystem m_AISystem;
	//DebugLineComponentManager m_DebugLineComponentManager;

	// TODO: GameObjectManager is getting blobbed because of all the componentmanagers and systems, extract to another class

	// Will increase the size of the array by one, representing the game object
	// the value of the element will indicate its generation
	GrowArray<GO_GEN_TYPE, int> m_GameObjects;
#ifdef BENCHMARK
	Timer m_CollisionTimeStamp;
	Timer m_AITimeStamp;
	Timer m_MeshCompManagerTimeStamp;
	Timer m_BulletsTimeStamp;
	friend class Profiler;
#endif // BENCHMARK

};
