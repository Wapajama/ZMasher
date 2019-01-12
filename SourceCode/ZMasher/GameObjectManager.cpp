#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
	: m_BulletSystem(&m_BulletCompManager, &m_TransformManager)
	, m_CollisionSystem(&m_SphereCollisionCompManager, &m_MomentumComponentManager, &m_TransformManager)
	, m_AISystem(&m_AICompManager, &m_SphereCollisionCompManager, &m_MomentumComponentManager, &m_TransformManager)
	, m_GameObjects(1024)
{
	//m_CurrentID = NULL_GAME_OBJECT.m_ID + 1;
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Update(const float dt)
{
	UpdateAllComponentManagers();//removes dead components,TODO: name change?

#ifdef BENCHMARK
	//Profiler::Instance()->BeginTask(m_BulletsTimeStamp);
	m_BulletSystem.Simulate(dt);
	//Profiler::Instance()->EndTask(m_BulletsTimeStamp);

	Profiler::Instance()->BeginTask(m_CollisionTimeStamp);
	m_CollisionSystem.Simulate(dt);
	Profiler::Instance()->EndTask(m_CollisionTimeStamp);

	Profiler::Instance()->BeginTask(m_AITimeStamp);
	m_AISystem.Simulate(dt);
	Profiler::Instance()->EndTask(m_AITimeStamp);

	Profiler::Instance()->BeginTask(m_MeshCompManagerTimeStamp);
	m_MeshManager.Update(&m_TransformManager);//TODO: meshmanager has two update functions, refactor, rename
	Profiler::Instance()->EndTask(m_MeshCompManagerTimeStamp);
#else 
	m_BulletSystem.Simulate(dt);

	m_CollisionSystem.Simulate(dt);

	m_AISystem.Simulate(dt);

	m_MeshManager.Update(&m_TransformManager);//TODO: meshmanager has two update functions, refactor, rename
#endif // BENCHMARK

}

bool GameObjectManager::Init()
{
#ifdef BENCHMARK
	m_AITimeStamp = Profiler::Instance()->AddTask("AISystem");
	m_CollisionTimeStamp = Profiler::Instance()->AddTask("CollisionSystem");
	m_MeshCompManagerTimeStamp = Profiler::Instance()->AddTask("MeshComponentManager");
	//m_BulletsTimeStamp = Profiler::Instance()->AddTask("BulletSystem");
#endif // BENCHMARK

	m_AISystem.Init(nullptr);

	m_ComponentManagers.Add(&m_BulletCompManager);
	m_ComponentManagers.Add(&m_MeshManager);
	m_ComponentManagers.Add(&m_TransformManager);
	m_ComponentManagers.Add(&m_SphereCollisionCompManager);
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
}

GameObject GameObjectManager::CreateGameObject()
{
	//GameObject new_game_object;
	//new_game_object.m_ID = m_CurrentID;
	//GAME_OBJECT_TOGGLE_ALIVE_GO(new_game_object);

	//++m_CurrentID;
	//return new_game_object;

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

void GameObjectManager::Destroy(GameObject game_Object, bool remove_everywhere)
{
	if (!Alive(game_Object))
	{
		// It's already destroyed
		return;
	}
	const GO_ID_TYPE go_id = game_Object.Index();
	++m_GameObjects[go_id];
	m_FreeIndexes.Add(go_id);
	
	if (!remove_everywhere)
	{
		return;
	}

	//go through all componentmanagers and remove the gameobject
	for (int i = 0; i < m_ComponentManagers.Size(); i++)
	{
		m_ComponentManagers[i]->RemoveComponentWithGameObject(game_Object, true);
	}
}
