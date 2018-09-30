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

bool BulletSystem::Simulate(const float dt, GameObjectManager* go_man)
{
	for (short i = 0; i < m_BulletCompManager->m_Bullets.Size(); i++)
	{
		BulletComponent& bullet = m_BulletCompManager->m_Bullets[i];
		if (!GAME_OBJECT_IS_ALIVE(bullet.m_GameObject))
		{
			continue;
		}
	 	ZMasher::Matrix44f* trans = m_TransformCompManager->GetTransform(bullet.m_GameObject);
		if (!trans)
		{
			//GAME_OBJECT_KILL(m_TransformCompManager->GetTransformComp(bullet.m_GameObject)->m_GameObject);

			go_man->Destroy(m_TransformCompManager->GetTransformComp(bullet.m_GameObject)->m_GameObject);
		}
		bullet.m_LifeTime -= dt;
		if (bullet.m_LifeTime <= 0.f)
		{
			go_man->Destroy(bullet.m_GameObject);
		}
	}

	return true;
}
