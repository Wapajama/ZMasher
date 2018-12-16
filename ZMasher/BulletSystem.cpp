#include "BulletSystem.h"
#include "TransformComponentManager.h"
#include "BulletComponentManager.h"
#include "GameObjectManager.h"
#include "GameplayState.h"

BulletSystem::BulletSystem(BulletComponentManager* bullet_comp_manager, TransformComponentManager* transform_comp_manager)
{
	m_BulletCompManager = bullet_comp_manager;
	m_TransformCompManager = transform_comp_manager;
}


BulletSystem::~BulletSystem()
{
}

bool BulletSystem::Simulate(const float dt)
{
	for (short i = 0; i < m_BulletCompManager->m_Components.Size(); i++)
	{
		BulletComponent& bullet = m_BulletCompManager->m_Components[i];
		if (!GameObjectManager::Instance()->Alive(bullet.m_GameObject))
		{
			continue;
		}
		bullet.m_LifeTime -= dt;
		if (bullet.m_LifeTime <= 0.f)
		{
			GameObjectManager::Instance()->Destroy(bullet.m_GameObject);
		}
	}

	return true;
}
