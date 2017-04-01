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
	 	ZMasher::Matrix44f* trans = m_TransformCompManager->GetTransform(m_BulletCompManager->m_Bullets[i].m_GameObject);
		//ASSERT(trans != nullptr, "BulletSystem::Simulate(const float dt), trans is nullptr!");
		if (trans)
		{
			trans->SetTranslation(trans->GetTranslation() + trans->GetVectorForward()*m_BulletCompManager->m_Bullets[i].m_Speed);
			m_BulletCompManager->m_Bullets[i].m_LifeTime -= dt;
		}
		else
		{
			m_BulletCompManager->m_Bullets[i].m_LifeTime = -1;
		}
		if (m_BulletCompManager->m_Bullets[i].m_LifeTime <= 0.f)
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(m_BulletCompManager->m_Bullets[i].m_GameObject);
		}
		if (trans && !GAME_OBJECT_IS_ALIVE(m_BulletCompManager->m_Bullets[i].m_GameObject))
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(m_TransformCompManager->GetTransformComp(m_BulletCompManager->m_Bullets[i].m_GameObject)->m_GameObject);
		}
	}

	return true;
}
