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
		m_Bullets.Add({ game_object, speed, damage, life_time });
	}
	else
	{
		m_Bullets[free_index] = { game_object, speed, damage, life_time };
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

void BulletComponentManager::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	for (int i = 0; i < m_Bullets.Size(); i++)
	{
		if (m_Bullets[i].m_GameObject==object)
		{
			//GAME_OBJECT_IS_ALIVE(m_Bullets[i].m_GameObject) ? GAME_OBJECT_TOGGLE_ALIVE_GO(m_Bullets[i].m_GameObject) : 0;
			GAME_OBJECT_KILL(m_Bullets[i].m_GameObject);
			break;
		}
	}
}