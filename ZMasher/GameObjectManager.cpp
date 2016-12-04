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

GameObject GameObjectManager::CreateGameObject()
{
	++m_CurrentID;
	GameObject new_game_object;
	new_game_object.m_ID = m_CurrentID;

	return new_game_object;
}

bool GameObjectManager::IsAlive(GameObject game_object)
{
	return true;
}

void GameObjectManager::Destroy(GameObject game_Object)
{
}

