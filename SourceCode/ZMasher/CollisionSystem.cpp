#include "CollisionSystem.h"
#include <ZMasher\SphereCollisionComponentManager.h>
#include <ZMasher\AABBComponentManager.h>
#include <ZMasher\MomentumComponentManager.h>
#include <ZMasher\TransformComponentManager.h>
#include <ZMasher\GameObjectManager.h>
#include <ZMasherGfxDX11\ZMModelFactory.h>

CollisionSystem::CollisionSystem(SphereCollisionComponentManager* sphere_collision_comp_manager,
	AABBComponentManager* aabb_component_manager,
	MomentumComponentManager* momentum_comp_manager,
	TransformComponentManager* transform_comp_manager)
	: m_SphereCollisionCompManager(sphere_collision_comp_manager)
	, m_AABBComponentManager(aabb_component_manager)
	, m_MomentumCompManager(momentum_comp_manager)
	, m_TransformCompManager(transform_comp_manager)
	, m_CollInfos(1024)
	, m_DebugLines(1024)
{
	m_SingleCollisionTimeStamp = Profiler::Instance()->AddTask("SingleCollision");
	m_DebugLines.Resize(1024 * 128);
}

static float SqDistToAABB(const ZMasher::Vector4f& pos_a,
	const AABBCollisionComponent& aabb,
	const ZMasher::Vector4f& pos_aabb)
{
	float p[3] = { pos_a.x, pos_a.y, pos_a.z };
	float b[3] = { pos_aabb.x, pos_aabb.y, pos_aabb.z };
	const float* r = aabb.r;
	float sqDist = 0.f;
	for (short i = 0; i < 3; i++)
	{
		float v = p[i];
		if (v < (b[i] - r[i])) sqDist += ((b[i] - r[i]) - v) * ((b[i] - r[i]) - v);
		if (v > (b[i] + r[i])) sqDist += (v - (b[i] - r[i])) * (v - (b[i] - r[i]));
	}
	return sqDist;
}

static bool SphereVsSphereTest(const SphereCollisionComponent& sphere_a, const SphereCollisionComponent& sphere_b,
	const ZMasher::Vector4f& pos_a, const ZMasher::Vector4f& pos_b, float& diff)
{
	using namespace ZMasher;
	const Vector4f closest_a_pos = Vector4Add(Vector4MulScalar((Vector4Sub(pos_b, pos_a)).GetNormal(), sphere_a.m_Radius), pos_a);
	diff = Vector4Sub(closest_a_pos, pos_b).Length() - sphere_b.m_Radius;
	return diff < 0;
}

static bool SphereVsAABBTest(const SphereCollisionComponent& sphere, const AABBCollisionComponent& aabb,
	const ZMasher::Vector4f& pos_sphere, const ZMasher::Vector4f& pos_aabb, float& sqDistRef)
{
	const float sqDist = SqDistToAABB(pos_sphere, aabb, pos_aabb);
	sqDistRef = sqDist;
	return sqDist <= sphere.m_Radius * sphere.m_Radius;
}

CollisionSystem::~CollisionSystem()
{
}

bool CollisionSystem::Simulate(const float dt)
{
	m_CollInfos.RemoveAll();

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
		ZMasher::Matrix44f* transformA = m_TransformCompManager->GetTransform(object_a);
		Profiler::Instance()->BeginTask(m_SingleCollisionTimeStamp);
		for (int j = i; j < m_SphereCollisionCompManager->m_Components.Size(); ++j)
		{
			SphereCollisionComponent& sphereB = m_SphereCollisionCompManager->m_Components[j];
			const GameObject object_b = sphereB.m_GameObject;
			if (!GameObjectManager::Instance()->Alive(object_b))
			{
				continue;
			}
			float diff = 0.f;
			ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(object_b);
			if (transformA &&
				transformB &&
				SphereVsSphereTest(sphereA,
					sphereB,
					transformA->GetTranslation(),
					transformB->GetTranslation(), diff))
			{
				MomentumComponent* mom_a = m_MomentumCompManager->GetComponent(object_a);
				MomentumComponent* mom_b = m_MomentumCompManager->GetComponent(object_b);

				(*sphereA.m_CollisionCallback)({ &sphereA, &sphereB, mom_a, mom_b });
				sphereA.collInfoIndex = m_CollInfos.Size();
				sphereB.collInfoIndex = m_CollInfos.Size();
				m_CollInfos.Add(
					{ object_a,
					object_b,
					transformA->GetTranslation().ToVector3f(),
					transformB->GetTranslation().ToVector3f() });

				ZMasher::Vector4f dir = (transformA->GetTranslation() - transformB->GetTranslation()).GetNormal()*(diff*0.5f);
				dir.y = 0; // don't want them to get pushed over or under
				transformA->SetTranslation(transformA->GetTranslation() - dir);

				dir = (transformB->GetTranslation() - transformA->GetTranslation()).GetNormal()*(diff*0.5f);
				dir.y = 0; // don't want them to get pushed over or under
				transformB->SetTranslation(transformB->GetTranslation() - dir);

			}
		}
		QuerySphereAgainstAllAABBS(sphereA, transformA);
		Profiler::Instance()->EndTask(m_SingleCollisionTimeStamp);
	}
	DrawDebugLines();
	return SimulatePhysics(dt);
}

void CollisionSystem::QuerySphereAgainstAllAABBS(const SphereCollisionComponent& sphere, ZMasher::Matrix44f* transform)
{
	for (int i = 0; i < m_AABBComponentManager->m_Components.Size(); i++)
	{
		AABBCollisionComponent& aabb = m_AABBComponentManager->m_Components[i];
		GameObject aabb_object = aabb.m_GameObject;
		if (!GameObjectManager::Instance()->Alive(aabb_object))
		{
			continue;
		}
		ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(aabb_object);
		float sqDist = 0.f;
		if (SphereVsAABBTest(sphere, aabb, transform->GetTranslation(), transformB->GetTranslation(), sqDist))
		{
			// Sphere and box are intersecting, seperate them.  

			ZMasher::Vector4f dir = (transform->GetTranslation() - transformB->GetTranslation()).GetNormal()*(sqrt(sqDist) - sphere.m_Radius);
			dir.y = 0; // don't want them to get pushed over or under the boxes
			transform->SetTranslation(transform->GetTranslation() - dir);
		}

	}
}

void CollisionSystem::DrawDebugLines()
{
	// Spheres
	for (int i = 0; i < m_DebugLines.Size(); ++i)
	{
		ZMModelFactory::Instance()->RemoveDebugLine(m_DebugLines[i]);
	}
	m_DebugLines.RemoveAll();

	DrawSpheres();
	DrawAABBs();
}

void CollisionSystem::DrawAABBs()
{
	for (int i = 0; i < m_AABBComponentManager->m_Components.Size(); ++i)
	{
		AABBCollisionComponent aabb = m_AABBComponentManager->m_Components[i];
		ZMasher::Vector3f pos = m_TransformCompManager->GetTransform(aabb.m_GameObject)->GetTranslation().ToVector3f();
		const float x = aabb.r[0];
		const float y = aabb.r[1];
		const float z = aabb.r[2];
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(x, y, -z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(x, -y, z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(-x, y, z),
			eColour::BLUE));

		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, -y, -z),
			pos + ZMasher::Vector3f(-x, -y, z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, -y, -z),
			pos + ZMasher::Vector3f(-x, y, -z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, -y, -z),
			pos + ZMasher::Vector3f(x, -y, -z),
			eColour::BLUE));

		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, -y, z),
			pos + ZMasher::Vector3f(-x, -y, z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, -y, z),
			pos + ZMasher::Vector3f(x, -y, -z),
			eColour::BLUE));

		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, -z),
			pos + ZMasher::Vector3f(x, y, -z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, -z),
			pos + ZMasher::Vector3f(-x, y, z),
			eColour::BLUE));

		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, z),
			pos + ZMasher::Vector3f(-x, -y, z),
			eColour::BLUE));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, -z),
			pos + ZMasher::Vector3f(x, -y, -z),
			eColour::BLUE));
	}
}

void CollisionSystem::DrawSpheres()
{
	for (int i = 0; i < m_SphereCollisionCompManager->m_Components.Size(); ++i)
	{
		SphereCollisionComponent sphere = m_SphereCollisionCompManager->m_Components[i];
		ZMasher::Vector3f pos = m_TransformCompManager->GetTransform(sphere.m_GameObject)->GetTranslation().ToVector3f();
		const float radius = sphere.m_Radius;
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(radius, 0, 0), pos + ZMasher::Vector3f(-radius, 0, 0), eColour::RED));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(0, radius, 0), pos + ZMasher::Vector3f(0, -radius, 0), eColour::RED));
		m_DebugLines.Add(ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(0, 0, radius), pos + ZMasher::Vector3f(0, 0, -radius), eColour::RED));
	}
}

CollisionInfoStruct * CollisionSystem::GetCollisionInfo(const int index)
{
	if (index >= 0 &&
		index < m_CollInfos.Size())
	{
		return &m_CollInfos[index];
	}
	return nullptr;
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


// If the object died, make sure to clean up the components
//if (!GameObjectManager::Instance()->Alive(sphereA.m_GameObject))
//{
//	TransformComponent* trans = m_TransformCompManager->GetComponent(object_a);
//	MomentumComponent* mom = &m_MomentumCompManager->m_Components[i];

//	if (trans)
//	{
//		GameObjectManager::Instance()->Destroy(trans->m_GameObject, false);
//	}
//	if (mom)
//	{
//		GameObjectManager::Instance()->Destroy(mom->m_GameObject, false);
//	}
//	
//}
//if (!GameObjectManager::Instance()->Alive(sphereB.m_GameObject))
//{
//	TransformComponent* trans = m_TransformCompManager->GetComponent(object_b);
//	MomentumComponent* mom = &m_MomentumCompManager->m_Components[i];

//	if (trans)
//	{
//		GameObjectManager::Instance()->Destroy(trans->m_GameObject, false);
//	}
//	if (mom)
//	{
//		GameObjectManager::Instance()->Destroy(mom->m_GameObject, false);
//	}
//}