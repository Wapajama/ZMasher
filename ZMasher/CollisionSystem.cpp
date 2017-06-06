#include "CollisionSystem.h"
#include <ZMasher\CollisionComponentManager.h>
#include <ZMasher\TransformComponentManager.h>

CollisionSystem::CollisionSystem(CollisionComponentManager* collision_comp_manager,
								 TransformComponentManager* transform_comp_manager)
	: m_CollisionCompManager(collision_comp_manager)
	, m_TransformCompManager(transform_comp_manager)
{
	//list_ID = m_TransformCompManager->RegisterList(1024);
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
	//for (int i = 0; i < m_CollisionCompManager->m_Spheres.Size(); ++i)
	//{
	//	for (int j = i; j < m_CollisionCompManager->m_Spheres.Size(); ++j)
	//	{
	//		SphereCollisionComponent& sphereA = m_CollisionCompManager->m_Spheres[i];
	//		SphereCollisionComponent& sphereB = m_CollisionCompManager->m_Spheres[j];
	//		const GameObject object_a = sphereA.m_GameObject;
	//		const GameObject object_b = sphereB.m_GameObject;
	//		ZMasher::Matrix44f* transformA = m_TransformCompManager->GetTransform(object_a);
	//		ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(object_b);
	//		if (transformA &&
	//			transformB &&
	//			GAME_OBJECT_IS_ALIVE(object_a) &&
	//			GAME_OBJECT_IS_ALIVE(object_b) &&
	//			SphereVsSphereTest(sphereA,
	//							   sphereB,
	//							   transformA->GetTranslation(),
	//							   transformB->GetTranslation()))
	//		{
	//			//m_CollisionCompManager->m_Collisions.Add(CollisionInfoComponent(m_CollisionCompManager->m_Spheres[i].m_GameObject,
	//			//																m_CollisionCompManager->m_Spheres[j].m_GameObject));
	//			//m_CollisionCompManager->m_Collisions.Add(CollisionInfoComponent(m_CollisionCompManager->m_Spheres[j].m_GameObject,
	//			//																m_CollisionCompManager->m_Spheres[i].m_GameObject));
	//			
	//			//GAME_OBJECT_TOGGLE_ALIVE_GO(m_CollisionCompManager->m_Spheres[i].m_GameObject);
	//			//GAME_OBJECT_TOGGLE_ALIVE_GO(m_CollisionCompManager->m_Spheres[j].m_GameObject);

	//			//GAME_OBJECT_TOGGLE_ALIVE_GO(m_TransformCompManager->GetTransformComp(object_a)->m_GameObject);
	//			//GAME_OBJECT_TOGGLE_ALIVE_GO(m_TransformCompManager->GetTransformComp(object_b)->m_GameObject);
	//		}
	//	}
	//}
	return SimulatePhysics(dt);
}

bool CollisionSystem::SimulatePhysics(const float dt)
{
	const float frame_time = dt;
	for (short i = 0; i < m_CollisionCompManager->m_Momentums.Size(); i++)
	{
		GameObject game_object = m_CollisionCompManager->m_Momentums[i].m_GameObject;
		ZMasher::Matrix44f* transform = m_TransformCompManager->GetTransform(game_object);
		if (transform == nullptr)
		{
			continue;
		}
		transform->SetTranslation(
			transform->GetTranslation() + 
			ZMasher::Vector4f(m_CollisionCompManager->m_Momentums[i].m_Speed*frame_time));
	}
	return true;
}