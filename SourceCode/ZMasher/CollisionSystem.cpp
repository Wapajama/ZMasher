#include "CollisionSystem.h"
#include <ZMasher\SphereCollisionComponentManager.h>
#include <ZMasher\AABBComponentManager.h>
#include <ZMasher\MomentumComponentManager.h>
#include <ZMasher\TransformComponentManager.h>
#include <ZMasher\GameObjectManager.h>
#include <ZMasherGfxDX11\ZMModelFactory.h>
#define COLL_N_TIMERS 10
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
	, m_Queries(1024)
	, m_SphereQueries(1024)
	, m_TimeStamps(128)
{
	m_DebugLines.Resize(1024 * 128);
	for (int i = 0; i < COLL_N_TIMERS; i++)
	{
		m_TimeStamps.Add(Timer());
	}
}

static float SqDistToAABB(const ZMasher::Vector4f& pos_a,
	const float* r,
	const ZMasher::Vector4f& pos_aabb)
{
	float p[3] = { pos_a.x, pos_a.y, pos_a.z };
	float b[3] = { pos_aabb.x, pos_aabb.y, pos_aabb.z };
	float sqDist = 0.f;
	for (short i = 0; i < 3; i++)
	{
		float v = p[i];
		if (v < (b[i] - r[i])) sqDist += ((b[i] - r[i]) - v) * ((b[i] - r[i]) - v);
		if (v > (b[i] + r[i])) sqDist += (v - (b[i] - r[i])) * (v - (b[i] - r[i]));
	}
	return sqDist;
}

static bool SphereVsSphereTest(float r_a, float r_b,
	const ZMasher::Vector4f& pos_a, const ZMasher::Vector4f& pos_b)
{
	using namespace ZMasher;
	Vector4f d = pos_a - pos_b;
	d.w = 0.f;
	const float dist2 = Dot(d, d);

	float radSum = r_a + r_b;
	return dist2 <= radSum * radSum;
}

static bool SphereVsAABBTest(float r_a, float* r_b,
	const ZMasher::Vector4f& pos_sphere, const ZMasher::Vector4f& pos_aabb, float& sqDistRef)
{
	const float sqDist = SqDistToAABB(pos_sphere, r_b, pos_aabb);
	sqDistRef = sqDist;
	return sqDist <= r_a * r_a;
}

static void ZombieCollisionFeedback(ZMasher::Matrix44f* transformA, ZMasher::Matrix44f* transformB, const float diff)
{
	ZMasher::Vector4f dir = (transformA->GetTranslation() - transformB->GetTranslation()).GetNormal()*(diff*0.5f);
	dir.y = 0; // don't want them to get pushed over or under
	transformA->SetTranslation(transformA->GetTranslation() - dir);
}

CollisionSystem::~CollisionSystem()
{
}

static GrowArray<std::string> indexes;

void CollisionSystem::Destroy()
{
	Profiler::Instance()->AddTimeStamp(m_SingleCollisionTimeStamp, "SingleCollision");
	Profiler::Instance()->AddTimeStamp(m_QueriesTimeStamp, "Collision Queries");
	Profiler::Instance()->AddTimeStamp(m_SimulatePhysicsTimeStamp, "Simulate Physics");
	Profiler::Instance()->AddTimeStamp(m_DrawDebugLinesTimeStamp, "Draw Debug Lines collisionManager");
	
	for (int i = 0; i < COLL_N_TIMERS; i++)
	{
		indexes.Add(("CollisionID: " + std::to_string(i)));
		Profiler::Instance()->AddTimeStamp(m_TimeStamps[i], indexes.GetLast().c_str());
	}
}

bool CollisionSystem::Simulate(const float dt)
{
	m_CollInfos.RemoveAll();

	m_SimulatePhysicsTimeStamp.StartTimeStamp();
	SimulatePhysics(dt);
	m_SimulatePhysicsTimeStamp.EndTimeStamp();

	m_QueriesTimeStamp.StartTimeStamp();
	ResolveQueries();
	m_QueriesTimeStamp.EndTimeStamp();

	//BRUTEFOOOOOOOOOOOOOOOOOOOORCE
	//TODO: Optimize this, might be a major task
	for (int i = 0; i < m_SphereCollisionCompManager->m_Components.Size(); ++i)
	{
		m_TimeStamps[0].StartTimeStamp();
		SphereCollisionComponent& sphereA = m_SphereCollisionCompManager->m_Components[i];
		m_TimeStamps[0].EndTimeStamp();

		const GameObject object_a = sphereA.m_GameObject;
		m_TimeStamps[1].StartTimeStamp();
		if (!GameObjectManager::Instance()->Alive(object_a))
		{
			m_TimeStamps[1].EndTimeStamp();
			continue;
		}
		m_TimeStamps[1].EndTimeStamp();

		m_TimeStamps[2].StartTimeStamp();
		ZMasher::Matrix44f* transformA = m_TransformCompManager->GetTransform(object_a);
		m_TimeStamps[2].EndTimeStamp();
		
		for (int j = i; j < m_SphereCollisionCompManager->m_Components.Size(); ++j)
		{
			SphereCollisionComponent& sphereB = m_SphereCollisionCompManager->m_Components[j];
			const GameObject object_b = sphereB.m_GameObject;
			m_TimeStamps[3].StartTimeStamp();
			if (object_a == object_b ||
				!GameObjectManager::Instance()->Alive(object_b))
			{
				m_TimeStamps[3].EndTimeStamp();
				continue;
			}
			m_TimeStamps[3].EndTimeStamp();
			m_TimeStamps[4].StartTimeStamp();
			ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(object_b);
			m_TimeStamps[4].EndTimeStamp();

			if (transformA == nullptr ||
				transformB == nullptr)
			{
				continue;
			}

			ZMasher::Vector4f ta = transformA->GetTranslation();
			ZMasher::Vector4f tb = transformB->GetTranslation();
			m_SingleCollisionTimeStamp.StartTimeStamp();
			const bool collisionResult = SphereVsSphereTest(sphereA.m_Radius,
															sphereB.m_Radius,
															ta, tb);
			m_SingleCollisionTimeStamp.EndTimeStamp();
			if (collisionResult)
			{
				m_TimeStamps[5].StartTimeStamp();
				MomentumComponent* mom_a = m_MomentumCompManager->GetComponent(object_a);
				m_TimeStamps[5].EndTimeStamp();
				m_TimeStamps[6].StartTimeStamp();
				MomentumComponent* mom_b = m_MomentumCompManager->GetComponent(object_b);
				m_TimeStamps[6].EndTimeStamp();

				m_TimeStamps[7].StartTimeStamp();
				(*sphereA.m_CollisionCallback)({ &sphereA, &sphereB, mom_a, mom_b });
				sphereA.collInfoIndex = m_CollInfos.Size();
				sphereB.collInfoIndex = m_CollInfos.Size();
				m_CollInfos.Add(
					{ object_a,
					object_b,
					ta.ToVector3f(),
					tb.ToVector3f() });
				m_TimeStamps[7].EndTimeStamp();

				ZMasher::Vector3f diffv = ta.ToVector3f() - tb.ToVector3f();
				const float diff = diffv.Length() - (sphereA.m_Radius + sphereB.m_Radius);

				m_TimeStamps[8].StartTimeStamp();
				ZombieCollisionFeedback(transformA, transformB, diff);
				ZombieCollisionFeedback(transformB, transformA, diff);
				m_TimeStamps[8].EndTimeStamp();
			}
		}
		m_TimeStamps[9].StartTimeStamp();
		QuerySphereAgainstAllAABBS(sphereA, transformA);
		m_TimeStamps[9].EndTimeStamp();
		
	}
	m_DrawDebugLinesTimeStamp.StartTimeStamp();
	DrawDebugLines();
	m_DrawDebugLinesTimeStamp.EndTimeStamp();
	return true;
}

void CollisionSystem::ResolveQueries()
{
	for (int i = 0; i < m_Queries.Size(); i++)
	{
		m_Queries[i].target = NULL_GAME_OBJECT;
		float shortestDiff = 9999999;
		switch (m_Queries[i].args->type)
		{
		case eSPHERE:
		{
			
			SphereQueryArgs* args = reinterpret_cast<SphereQueryArgs*>(m_Queries[i].args);
			for (int o = 0; o < m_SphereCollisionCompManager->m_Components.Size(); o++)
			{
				SphereCollisionComponent s = m_SphereCollisionCompManager->m_Components[o];
				if (s.m_GameObject == m_Queries[i].owner ||
					s.m_CollisionFilter == eCOLLISIONTYPE::eTurretBullet)
				{
					continue;
				}
				ZMasher::Vector4f pos = m_TransformCompManager->GetTransform(s.m_GameObject)->GetTranslation();
				if (SphereVsSphereTest(args->r, s.m_Radius, args->position, pos))
				{
					ZMasher::Vector3f diffv = pos.ToVector3f() - args->position.ToVector3f();
					const float diff = diffv.Length() - (args->r + s.m_Radius);

					if (diff < shortestDiff)
					{
						shortestDiff = diff;
						m_Queries[i].target = s.m_GameObject;
					}
				}
			}
		}
		default:
			break;
		}
	}
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
		if (SphereVsAABBTest(sphere.m_Radius, aabb.r, transform->GetTranslation(), transformB->GetTranslation(), sqDist))
		{
			// Sphere and box are intersecting, separate them.
			ZombieCollisionFeedback(transform, transformB, sqrt(sqDist) - sphere.m_Radius);
		}

	}
}

void CollisionSystem::DrawDebugLines()
{
	//for (int i = 0; i < m_DebugLines.Size(); ++i)
	//{
	//	ZMModelFactory::Instance()->RemoveDebugLine(m_DebugLines[i]);
	//}
	//m_DebugLines.RemoveAll();

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
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(x, y, -z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(x, -y, z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(-x, y, z),
			eColour::BLUE));

		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, -y, -z),
			pos + ZMasher::Vector3f(-x, -y, z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, -y, -z),
			pos + ZMasher::Vector3f(-x, y, -z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, -y, -z),
			pos + ZMasher::Vector3f(x, -y, -z),
			eColour::BLUE));

		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, -y, z),
			pos + ZMasher::Vector3f(-x, -y, z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, -y, z),
			pos + ZMasher::Vector3f(x, -y, -z),
			eColour::BLUE));

		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, -z),
			pos + ZMasher::Vector3f(x, y, -z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, -z),
			pos + ZMasher::Vector3f(-x, y, z),
			eColour::BLUE));

		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, z),
			pos + ZMasher::Vector3f(-x, -y, z),
			eColour::BLUE));
		(ZMModelFactory::Instance()->CreateDebugLine(
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
		DrawSphere(radius, pos);
	}
	for (int i = 0; i < m_Queries.Size(); i++)
	{
		switch (m_Queries[i].args->type)
		{
		case eSPHERE:
		{
			SphereQueryArgs* args = reinterpret_cast<SphereQueryArgs*>(m_Queries[i].args);
			DrawSphere(args->r, args->position.ToVector3f());
		}
		default:
			break;
		}
	}
}

void CollisionSystem::DrawSphere(const float radius, const ZMasher::Vector3f pos)
{
	(ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(radius, 0, 0), pos + ZMasher::Vector3f(-radius, 0, 0), eColour::RED));
	(ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(0, radius, 0), pos + ZMasher::Vector3f(0, -radius, 0), eColour::RED));
	(ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(0, 0, radius), pos + ZMasher::Vector3f(0, 0, -radius), eColour::RED));
}

CollisionInfoStruct* CollisionSystem::GetCollisionInfo(const int index)
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

CollisionQuery* CollisionSystem::CreateQuery(eCOLLISIONTYPE type, GameObject owner, float radius, ZMasher::Vector3f pos)
{
	SphereQueryArgs sq;
	sq.type = eSPHERE;
	sq.position = m_TransformCompManager->GetComponent(owner)->m_Transform.GetTranslation();
	sq.r = radius;
	m_SphereQueries.Add(sq);

	CollisionQuery cq;
	cq.args = &m_SphereQueries.GetLast();
	cq.owner = owner;
	cq.target = NULL_GAME_OBJECT;
	m_Queries.Add(cq);
	return &m_Queries.GetLast();
}


CollisionQuery* CollisionSystem::GetQuery(GameObject owner, int id)
{
	for (int i = 0; i < m_Queries.Size(); i++)
	{
		if (m_Queries[i].owner == owner)
		{
			return &m_Queries[i];
		}
	}
	return nullptr;
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
