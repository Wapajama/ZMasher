#include "CollisionSystem.h"
#include <ZMasher\CollisionComponentManager.h>
#include <ZMasher\TransformComponentManager.h>

CollisionSystem::CollisionSystem(CollisionComponentManager* collision_comp_manager,
								 TransformComponentManager* transform_comp_manager)
	: m_CollisionCompManager(collision_comp_manager)
	, m_TransformCompManager(transform_comp_manager)
{
	m_SingleCollisionTimeStamp = Profiler::Instance()->AddTask("SingleCollision");
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
	for (int i = 0; i < m_CollisionCompManager->m_Spheres.Size(); ++i)
	{
		SphereCollisionComponent& sphereA = m_CollisionCompManager->m_Spheres[i];
		const GameObject object_a = sphereA.m_GameObject;
		if (!GAME_OBJECT_IS_ALIVE(object_a))
		{
			continue;
		}
		Profiler::Instance()->BeginTask(m_SingleCollisionTimeStamp);
		for (int j = i; j < m_CollisionCompManager->m_Spheres.Size(); ++j)
		{
			SphereCollisionComponent& sphereB = m_CollisionCompManager->m_Spheres[j];
			const GameObject object_b = sphereB.m_GameObject;
			if (!GAME_OBJECT_IS_ALIVE(object_b))
			{
				continue;
			}
			ZMasher::Matrix44f* transformA = m_TransformCompManager->GetTransform(object_a);
			ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(object_b);
			if (transformA &&
				transformB &&
				SphereVsSphereTest(sphereA,
								   sphereB,
								   transformA->GetTranslation(),
								   transformB->GetTranslation()))
			{
				(*sphereA.m_CollisionCallback)({ &sphereA, &sphereB, nullptr, nullptr });
				if (!GAME_OBJECT_IS_ALIVE(sphereA.m_GameObject))
				{
					TransformComponent* trans = m_TransformCompManager->GetTransformComp(object_a);
					MomentumComponent* mom = & m_CollisionCompManager->m_Momentums[i];
			
					if (GAME_OBJECT_IS_ALIVE(trans->m_GameObject))
					{
						GAME_OBJECT_TOGGLE_ALIVE_GO(trans->m_GameObject);
					}
					if (GAME_OBJECT_IS_ALIVE(mom->m_GameObject))
					{
						GAME_OBJECT_TOGGLE_ALIVE_GO(mom->m_GameObject);
					}
				}
				if (!GAME_OBJECT_IS_ALIVE(sphereB.m_GameObject))
				{
					TransformComponent* trans = m_TransformCompManager->GetTransformComp(object_b);
					MomentumComponent* mom = &m_CollisionCompManager->m_Momentums[j];

					if (GAME_OBJECT_IS_ALIVE(trans->m_GameObject))
					{
						GAME_OBJECT_TOGGLE_ALIVE_GO(trans->m_GameObject);
					}
					if (GAME_OBJECT_IS_ALIVE(mom->m_GameObject))
					{
						GAME_OBJECT_TOGGLE_ALIVE_GO(mom->m_GameObject);
					}
				}
			}
		}
		Profiler::Instance()->EndTask(m_SingleCollisionTimeStamp);
	}
	return SimulatePhysics(dt);
}

bool CollisionSystem::SimulatePhysics(const float dt)
{
	const float frame_time = dt;
	for (short i = 0; i < m_CollisionCompManager->m_Momentums.Size(); i++)
	{
		GameObject game_object = m_CollisionCompManager->m_Momentums[i].m_GameObject;
		if (!GAME_OBJECT_IS_ALIVE(game_object))//means that it's not removed, but waiting to be used by a new object
		{
			continue;
		}
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