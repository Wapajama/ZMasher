#include "CollisionSystem.h"
#include <ZMasher\CollisionComponentManager.h>
#include <ZMasher\TransformComponentManager.h>

CollisionSystem::CollisionSystem(CollisionComponentManager* collision_comp_manager,
								 TransformComponentManager* transform_comp_manager)
	: m_CollisionCompManager(collision_comp_manager)
	, m_TransformCompManager(transform_comp_manager)
{
}

static bool SphereVsSphereTest(const SphereCollisionComponent& sphere_a, const SphereCollisionComponent& sphere_b,
							   const ZMasher::Vector4f& pos_a, const ZMasher::Vector4f& pos_b)
{
	using namespace ZMasher;
	const Vector4f closest_a_pos = Vector4Add( Vector4MulScalar((Vector4Sub(pos_b, pos_a)).GetNormal(), sphere_a.m_Radius) , pos_a);
	return Vector4Sub(closest_a_pos, pos_b).Length() < sphere_b.m_Radius;
}

CollisionSystem::~CollisionSystem()
{
}

bool CollisionSystem::Simulate(const float dt)
{
	//BRUTEFOOOOOOOOOOOOOOOOOOOORCE
	//TODO: Optimize this, might be a major task
	for (int i = 0; i < m_CollisionCompManager->m_Spheres.Size(); ++i)
	{
		for (int j = 0; j < m_CollisionCompManager->m_Spheres.Size(); ++j)
		{
			const GameObject object_a = m_CollisionCompManager->m_Spheres[i].m_GameObject;
			const GameObject object_b = m_CollisionCompManager->m_Spheres[j].m_GameObject;
			if (m_TransformCompManager->GetTransform(object_a) &&
				m_TransformCompManager->GetTransform(object_b) &&
				GAME_OBJECT_IS_ALIVE(object_a) &&
				GAME_OBJECT_IS_ALIVE(object_b) &&
				SphereVsSphereTest(m_CollisionCompManager->m_Spheres[i],
								   m_CollisionCompManager->m_Spheres[j],
								   m_TransformCompManager->GetTransform(object_a)->GetTranslation(),
								   m_TransformCompManager->GetTransform(object_b)->GetTranslation()))
			{
				//m_CollisionCompManager->m_Collisions.Add(CollisionInfoComponent(m_CollisionCompManager->m_Spheres[i].m_GameObject,
				//																m_CollisionCompManager->m_Spheres[j].m_GameObject));
				//m_CollisionCompManager->m_Collisions.Add(CollisionInfoComponent(m_CollisionCompManager->m_Spheres[j].m_GameObject,
				//																m_CollisionCompManager->m_Spheres[i].m_GameObject));
				
				//GAME_OBJECT_TOGGLE_ALIVE_GO(m_CollisionCompManager->m_Spheres[i].m_GameObject);
				//GAME_OBJECT_TOGGLE_ALIVE_GO(m_CollisionCompManager->m_Spheres[j].m_GameObject);

				//GAME_OBJECT_TOGGLE_ALIVE_GO(m_TransformCompManager->GetTransformComp(object_a)->m_GameObject);
				//GAME_OBJECT_TOGGLE_ALIVE_GO(m_TransformCompManager->GetTransformComp(object_b)->m_GameObject);
			}
		}
	}
	return SimulatePhysics(dt);
}

bool CollisionSystem::SimulatePhysics(const float dt)
{
	const float frame_time = dt > 0.03 ? 0.016 : dt;
	for (short i = 0; i < m_CollisionCompManager->m_Momentums.Size(); i++)
	{
		GameObject game_object = m_CollisionCompManager->m_Momentums[i].m_GameObject;
		if (m_TransformCompManager->GetTransform(game_object) == nullptr)
		{
			continue;
		}
		m_TransformCompManager->GetTransform(game_object)->SetTranslation(
			m_TransformCompManager->GetTransform(game_object)->GetTranslation() + 
			ZMasher::Vector4f(m_CollisionCompManager->m_Momentums[i].m_Speed*frame_time));
	}
	return true;
}