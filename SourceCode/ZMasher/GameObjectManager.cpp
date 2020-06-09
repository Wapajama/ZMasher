#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
	: m_BulletSystem(&m_BulletCompManager, &m_TransformManager)
	, m_CollisionSystem(&m_SphereCollisionCompManager, &m_AABBComponentManager, &m_MomentumComponentManager, &m_TransformManager)
	, m_AISystem(&m_AICompManager, &m_SphereCollisionCompManager, &m_MomentumComponentManager, &m_TransformManager, &m_CollisionSystem)
{
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Update(const float dt)
{
	UpdateAllComponentManagers();//removes dead components,TODO: name change?

#ifdef BENCHMARK
	m_BulletSystem.Simulate(dt);

	m_CollisionTimeStamp.StartTimeStamp();
	m_CollisionSystem.Simulate(dt);
	m_CollisionTimeStamp.EndTimeStamp();

	m_AITimeStamp.StartTimeStamp();
	m_AISystem.Simulate(dt);
	m_AITimeStamp.EndTimeStamp();

	m_MeshCompManagerTimeStamp.StartTimeStamp();
	m_MeshManager.Update(&m_TransformManager);//TODO: meshmanager has two update functions, refactor, rename
	m_MeshCompManagerTimeStamp.EndTimeStamp();
#else 
	m_BulletSystem.Simulate(dt);

	m_AISystem.Simulate(dt);

	m_CollisionSystem.Simulate(dt);

	m_MeshManager.Update(&m_TransformManager);//TODO: meshmanager has two update functions, refactor, rename
#endif // BENCHMARK

}

bool GameObjectManager::Init()
{
	m_AISystem.Init(nullptr);

	m_ComponentManagers.Add(&m_BulletCompManager);
	m_ComponentManagers.Add(&m_MeshManager);
	m_ComponentManagers.Add(&m_TransformManager);
	m_ComponentManagers.Add(&m_SphereCollisionCompManager);
	m_ComponentManagers.Add(&m_AABBComponentManager);
	m_ComponentManagers.Add(&m_MomentumComponentManager);
	m_ComponentManagers.Add(&m_AICompManager);

	for (short i = 0; i < m_ComponentManagers.Size(); i++)
	{
		const bool result = m_ComponentManagers[i]->Init();
		ASSERT(result, "ComponentManager failed to init!");
	}
	return true;
}

void GameObjectManager::Destroy()
{
	#ifdef BENCHMARK
	Profiler::Instance()->AddTimeStamp(m_AITimeStamp, "AISystem");
	Profiler::Instance()->AddTimeStamp(m_CollisionTimeStamp, "CollisionSystem");
	Profiler::Instance()->AddTimeStamp(m_MeshCompManagerTimeStamp, "MeshComponentManager");
#endif // BENCHMARK

	for (int i = 0; i < m_ComponentManagers.Size(); i++)
	{
		m_ComponentManagers[i]->Destroy();
	}
	m_CollisionSystem.Destroy();
	m_AISystem.Destroy();
}

GameObject GameObjectManager::CreateGameObject()
{
	GameObject new_GameObject;
	if (m_FreeIndexes.Size() > 0)
	{
		new_GameObject.m_ID = m_FreeIndexes.GetLast();
		new_GameObject.m_Gen = m_GameObjects[new_GameObject.m_ID];
		m_FreeIndexes.RemoveLast();
	}
	else
	{
		m_GameObjects.Add(0);
		new_GameObject.m_ID = m_GameObjects.Size()-1;
		ASSERT(new_GameObject.m_ID < (static_cast<GO_ID_TYPE>(1) << GAMEOBJECT_ID_BITS),"Too many game objects!");
	}
	return new_GameObject;
}

bool GameObjectManager::Alive(GameObject game_object)
{
	return m_GameObjects[game_object.Index()] == game_object.Generation();
}

void GameObjectManager::UpdateAllComponentManagers()
{
	for (short i = 0; i < m_ComponentManagers.Size(); i++)
	{
		ASSERT(m_ComponentManagers[i]->Update(), "Failed to update a componentmanager!");
	}
}

void GameObjectManager::Destroy(GameObject game_Object, bool immediately)
{
	if (!Alive(game_Object))
	{
		// It's already destroyed
		return;
	}
	const GO_ID_TYPE go_id = game_Object.Index();
	++m_GameObjects[go_id];
	m_FreeIndexes.Add(go_id);

	//go through all componentmanagers and remove the gameobject
	for (int i = 0; i < m_ComponentManagers.Size(); i++)
	{
		m_ComponentManagers[i]->RemoveComponentWithGameObject(game_Object, true);
	}
}
