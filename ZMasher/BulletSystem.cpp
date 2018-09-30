#include "BulletSystem.h"
#include "TransformComponentManager.h"
#include "BulletComponentManager.h"


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
	for (short i = 0; i < m_BulletCompManager->m_Bullets.Size(); i++)
	{
		BulletComponent& bullet = m_BulletCompManager->m_Bullets[i];
		if (!GAME_OBJECT_IS_ALIVE(bullet.m_GameObject))
		{
			// A dead bullet that hasn't been removed yet
			continue;
		}
	 	ZMasher::Matrix44f* trans = m_TransformCompManager->GetTransform(bullet.m_GameObject);
		//ASSERT(trans != nullptr, "BulletSystem::Simulate(const float dt), trans is nullptr!");
		//if (trans)
		//{
			//trans->SetTranslation(trans->GetTranslation() + trans->GetVectorForward()*bullet.m_Speed);
			bullet.m_LifeTime -= dt;
		//}
		//else
		//{
		//	bullet.m_LifeTime = -1;
		//}
		if (bullet.m_LifeTime <= 0.f)
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(bullet.m_GameObject);
		}
		if (trans && !GAME_OBJECT_IS_ALIVE(bullet.m_GameObject))
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(m_TransformCompManager->GetTransformComp(bullet.m_GameObject)->m_GameObject);
		}
	}

	return true;
}
