#include "CollisionSystem.h"
#include <ZMasher\SphereCollisionComponentManager.h>
#include <ZMasher\MomentumComponentManager.h>
#include <ZMasher\TransformComponentManager.h>
#include <ZMasher\GameObjectManager.h>

CollisionSystem::CollisionSystem(SphereCollisionComponentManager* sphere_collision_comp_manager,
	MomentumComponentManager* momentum_comp_manager,
	TransformComponentManager* transform_comp_manager)
	: m_SphereCollisionCompManager(sphere_collision_comp_manager)
	, m_MomentumCompManager(momentum_comp_manager)
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
	for (int i = 0; i < m_SphereCollisionCompManager->m_Components.Size(); ++i)
	{
		SphereCollisionComponent& sphereA = m_SphereCollisionCompManager->m_Components[i];
		const GameObject object_a = sphereA.m_GameObject;
		if (!GameObjectManager::Instance()->Alive(object_a))
		{
			continue;
		}
		Profiler::Instance()->BeginTask(m_SingleCollisionTimeStamp);
		for (int j = i; j < m_SphereCollisionCompManager->m_Components.Size(); ++j)
		{
			SphereCollisionComponent& sphereB = m_SphereCollisionCompManager->m_Components[j];
			const GameObject object_b = sphereB.m_GameObject;
			if (!GameObjectManager::Instance()->Alive(object_b))
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
				// If the object died, make sure to clean up the components
				if (!GameObjectManager::Instance()->Alive(sphereA.m_GameObject))
				{
					TransformComponent* trans = m_TransformCompManager->GetComponent(object_a);
					MomentumComponent* mom = &m_MomentumCompManager->m_Components[i];
			
					if (trans)
					{
						GameObjectManager::Instance()->Destroy(trans->m_GameObject, false);
					}
					if (mom)
					{
						GameObjectManager::Instance()->Destroy(mom->m_GameObject, false);
					}
					
				}
				if (!GameObjectManager::Instance()->Alive(sphereB.m_GameObject))
				{
					TransformComponent* trans = m_TransformCompManager->GetComponent(object_b);
					MomentumComponent* mom = &m_MomentumCompManager->m_Components[i];

					if (trans)
					{
						GameObjectManager::Instance()->Destroy(trans->m_GameObject, false);
					}
					if (mom)
					{
						GameObjectManager::Instance()->Destroy(mom->m_GameObject, false);
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
	for (short i = 0; i < m_MomentumCompManager->m_Components.Size(); i++)
	{
		GameObject game_object = m_MomentumCompManager->m_Components[i].m_GameObject;
		if (!GameObjectManager::Instance()->Alive(game_object))//means that it's not removed, but waiting to be used by a new object
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
			ZMasher::Vector4f(m_MomentumCompManager->m_Components[i].m_Speed*frame_time));
	}
	return true;
}