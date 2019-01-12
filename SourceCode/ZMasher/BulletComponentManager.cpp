#include "BulletComponentManager.h"
#include <ZMasher\GameObjectManager.h>

BulletComponentManager::BulletComponentManager()
{
}

BulletComponentManager::~BulletComponentManager()
{
}

bool BulletComponentManager::AddComponent(const GameObject game_object, const float speed, const float damage, const float life_time)
{
	ComponentManager::AddComponent({ game_object, speed, damage, life_time }, game_object);
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
	return ComponentManager::Update();
}

//void BulletComponentManager::RemoveComponentWithGameObjectInternal(GameObject object)
//{
//	// This class doens't have any lookup yet. 
//	for (int i = 0; i < m_Bullets.Size(); i++)
//	{
//		if (m_Bullets[i].m_GameObject == object)
//		{
//			GameObjectManager::Instance()->Destroy(m_Bullets[i].m_GameObject, false);
//			m_Bullets.RemoveCyclic(i);
//			break;
//		}
//	}
//}
