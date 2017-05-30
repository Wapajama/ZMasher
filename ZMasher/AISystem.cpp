#include "AISystem.h"

#include <ZMasher/AIComponentManager.h>
#include <ZMasher/CollisionComponentManager.h>
#include <ZMasher/TransformComponentManager.h>


AISystem::AISystem(AIComponentManager* ai,
		CollisionComponentManager* collision,
		TransformComponentManager* transform)
	: m_AIMngr(ai)
	, m_CollisionMngr(collision)
	, m_TransformMngr(transform)
{
}


AISystem::~AISystem()
{
}

bool AISystem::Simulate(const float dt)
{
	//first try, everyone seeks 0,0,0
	const ZMasher::Vector3f target(0,0,0);//TODO: REMOVE
	for (short i = 0; i < m_AIMngr->m_AIComponents.Size(); i++)
	{
		const GameObject game_object = m_AIMngr->m_AIComponents[i].m_GameObject;
		if (m_TransformMngr->GetTransform(game_object) == nullptr)
		{
			continue;
		}
		//seek
		ZMasher::Vector3f direction;
		direction = target - ZMasher::Vector3f( m_TransformMngr->GetTransform(game_object)->GetTranslation());//TODO: refactor this line
		if (direction.Length() < 1.f)
		{
			m_CollisionMngr->GetMomentumComponent(game_object)->m_Speed = ZMasher::Vector3f(0,0,0);
			continue;
		}
		ZMasher::Vector3f test1(2,2,2);
		test1.Normalize();
		direction.Normalize();
		m_CollisionMngr->GetMomentumComponent(game_object)->m_Speed = direction * 5.f;
	}
	return true;
}