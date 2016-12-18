#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{
	m_CurrentID = 0;

}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::Update(const float dt)
{

}

bool GameObjectManager::Init()
{
	return true;
}

void GameObjectManager::Destroy()
{
}

GameObject GameObjectManager::CreateGameObject(const int components)
{
	GameObject new_game_object;
	new_game_object.m_ID = m_CurrentID;

	//last bit is the "alive" bit
	new_game_object.m_ID = new_game_object.m_ID | (1 << 31);

	++m_CurrentID;
	return new_game_object;
}

bool GameObjectManager::IsAlive(GameObject game_object)
{
	return game_object.m_ID & GAME_OBJECT_ALIVE_BIT;
}

void GameObjectManager::Destroy(GameObject& game_Object)
{

}

