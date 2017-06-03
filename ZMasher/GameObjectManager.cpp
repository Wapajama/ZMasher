#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
	: m_BulletSystem(&m_BulletCompManager, &m_TransformManager)
	, m_CollisionSystem(&m_CollisionCompManager, &m_TransformManager)
	, m_AISystem(&m_AICompManager, &m_CollisionCompManager, &m_TransformManager)
{
	m_CurrentID = 0;
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Update(const float dt)
{
	UpdateAllComponentManagers();//removes dead components,TODO: name change?

	m_BulletSystem.Simulate(dt);
	Profiler::Instance()->BeginTask(m_CollisionTimeStamp);
	m_CollisionSystem.Simulate(dt);
	Profiler::Instance()->EndTask(m_CollisionTimeStamp);

	Profiler::Instance()->BeginTask(m_AITimeStamp);
	m_AISystem.Simulate(dt);
	Profiler::Instance()->EndTask(m_AITimeStamp);

	Profiler::Instance()->BeginTask(m_MeshCompManagerTimeStamp);
	m_MeshManager.Update(&m_TransformManager);//TODO: meshmanager has two update functions, refactor, rename
	Profiler::Instance()->EndTask(m_MeshCompManagerTimeStamp);
}

bool GameObjectManager::Init()
{
#ifdef BENCHMARK
	m_AITimeStamp = Profiler::Instance()->AddTask("AISystem");
	m_CollisionTimeStamp = Profiler::Instance()->AddTask("CollisionSystem");
	m_MeshCompManagerTimeStamp = Profiler::Instance()->AddTask("MeshComponentManager");
#endif // BENCHMARK

	m_AISystem.Init(nullptr);

	m_ComponentManagers.Add(&m_BulletCompManager);
	m_ComponentManagers.Add(&m_MeshManager);
	m_ComponentManagers.Add(&m_TransformManager);
	m_ComponentManagers.Add(&m_CollisionCompManager);
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
	GameObject new_game_object;
	new_game_object.m_ID = m_CurrentID;

	//last bit is the "alive" bit
	new_game_object.m_ID = new_game_object.m_ID | (1 << 31);

	++m_CurrentID;
	return new_game_object;
}

void GameObjectManager::UpdateAllComponentManagers()
{
	for (short i = 0; i < m_ComponentManagers.Size(); i++)
	{
		ASSERT(m_ComponentManagers[i]->Update(), "Failed to update a componentmanager!");
	}
}

void GameObjectManager::Destroy(GameObject& game_Object)
{
	//go through all componentmanagers and remove the gameobject
	ASSERT(false, "not yet implemented!");
}
