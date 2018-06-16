#include "BulletComponentManager.h"



BulletComponentManager::BulletComponentManager():m_Bullets(128)
{
}


BulletComponentManager::~BulletComponentManager()
{
}

bool BulletComponentManager::AddComponent(const GameObject game_object, const float speed, const float damage, const float life_time)
{
	const int free_index = PopFreeIndex();

	if (free_index == m_FreeIndexes.found_none)
	{
		m_Bullets.Add(BulletComponent(game_object, speed, damage, life_time));
	}
	else
	{
		m_Bullets[free_index] = BulletComponent(game_object, speed, damage, life_time);
	}
	return true;
}

bool BulletComponentManager::Init()
{
	return true;
}

void BulletComponentManager::Destroy()
{
}

bool BulletComponentManager::Update()
{
	RemoveDeadComponents(m_Bullets);
	return true;
}