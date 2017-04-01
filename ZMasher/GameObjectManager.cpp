#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
	: m_BulletSystem(&m_BulletCompManager, &m_TransformManager)
	, m_CollisionSystem(&m_CollisionCompManager, &m_TransformManager)
{
	m_CurrentID = 0;
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Update(const float dt)
{
	UpdateAllComponentManagers();//removes dead components

	m_BulletSystem.Simulate(dt);
	m_CollisionSystem.Simulate(dt);

	m_MeshManager.Update(&m_TransformManager);
}

bool GameObjectManager::Init()
{
	m_ComponentManagers.Add(&m_BulletCompManager);
	m_ComponentManagers.Add(&m_MeshManager);
	m_ComponentManagers.Add(&m_TransformManager);
	m_ComponentManagers.Add(&m_CollisionCompManager);
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
}
