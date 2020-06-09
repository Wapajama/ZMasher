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
#ifdef BENCHMARK
	, m_TimeStamps(128)
#endif // BENCHMARK

{
	m_DebugLines.Resize(1024 * 128);
#ifdef BENCHMARK
	for (int i = 0; i < COLL_N_TIMERS; i++)
	{
		m_TimeStamps.Add(Timer());
	}
#endif // BENCHMARK

}

static float SqDistToAABB(const ZMasher::Vector4f& __restrict pos_a,
						  const float* r,
						  const ZMasher::Vector4f& __restrict pos_aabb)
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
							   const ZMasher::Vector4f& __restrict pos_a, const ZMasher::Vector4f& __restrict pos_b)
{
	using namespace ZMasher;
	Vector4f d = pos_a - pos_b;
	d.w = 0.f;
	const float dist2 = Dot(d, d);

	float radSum = r_a + r_b;
	return dist2 <= radSum * radSum;
}

static bool SphereVsAABBTest(float r_a, float* __restrict r_b,
							 const ZMasher::Vector4f& pos_sphere, const ZMasher::Vector4f& pos_aabb, float & __restrict sqDistRef)
{
	const float sqDist = SqDistToAABB(pos_sphere, r_b, pos_aabb);
	sqDistRef = sqDist;
	return sqDist <= r_a * r_a;
}

static void ZombieCollisionFeedback(ZMasher::Matrix44f* __restrict transformA, ZMasher::Matrix44f* __restrict transformB, const float diff)
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
#ifdef BENCHMARK
	Profiler::Instance()->AddTimeStamp(m_SingleCollisionTimeStamp, "SingleCollision");
	Profiler::Instance()->AddTimeStamp(m_QueriesTimeStamp, "Collision Queries");
	Profiler::Instance()->AddTimeStamp(m_SimulatePhysicsTimeStamp, "Simulate Physics");
	Profiler::Instance()->AddTimeStamp(m_DrawDebugLinesTimeStamp, "Draw Debug Lines collisionManager");

	for (int i = 0; i < COLL_N_TIMERS; i++)
	{
		indexes.Add(("CollisionID: " + std::to_string(i)));
		Profiler::Instance()->AddTimeStamp(m_TimeStamps[i], indexes.GetLast().c_str());
	}
#endif // BENCHMARK

}
#define START_TIME_STAMP(ts)
#define END_TIME_STAMP(ts)
bool CollisionSystem::Simulate(const float dt)
{
	m_CollInfos.RemoveAll();

	START_TIME_STAMP(m_SimulatePhysicsTimeStamp);
	SimulatePhysics(dt);
	END_TIME_STAMP(m_SimulatePhysicsTimeStamp);

	START_TIME_STAMP(m_QueriesTimeStamp);
	ResolveQueries();
	END_TIME_STAMP(m_QueriesTimeStamp);

	//BRUTEFOOOOOOOOOOOOOOOOOOOORCE
	//TODO: Optimize this, might be a major task
	for (int i = 0; i < m_SphereCollisionCompManager->m_Components.Size(); ++i)
	{
		SphereCollisionComponent sphereA = m_SphereCollisionCompManager->m_Components[i];

		const GameObject object_a = sphereA.m_GameObject;
		if (!GameObjectManager::Instance()->Alive(object_a))
		{
			continue;
		}
		const ZMasher::Matrix44f transformA = *m_TransformCompManager->GetTransform(object_a);

		for (int j = i+1; j < m_SphereCollisionCompManager->m_Components.Size(); ++j)
		{
			const SphereCollisionComponent sphereB = m_SphereCollisionCompManager->m_Components[j];
			const GameObject object_b = sphereB.m_GameObject;
			if (!GameObjectManager::Instance()->Alive(object_a) ||
				!GameObjectManager::Instance()->Alive(object_b))
			{
				continue;
			}

			const ZMasher::Matrix44f transformB = *m_TransformCompManager->GetTransform(object_b);

			const bool collisionResult = SphereVsSphereTest(sphereA.m_Radius,
															sphereB.m_Radius,
															transformA.GetTranslation(), transformB.GetTranslation());
			if (collisionResult)
			{
				const MomentumComponent mom_a = *m_MomentumCompManager->GetComponent(object_a);
				const MomentumComponent mom_b = *m_MomentumCompManager->GetComponent(object_b);
				
				ZMasher::Vector3f diffv =  transformA.GetTranslation().ToVector3f() - transformB.GetTranslation().ToVector3f();
				const float diff = diffv.Length() - (sphereA.m_Radius + sphereB.m_Radius);

				ZMasher::Matrix44f* mPointerA = m_TransformCompManager->GetTransform(object_a);
				ZMasher::Matrix44f* mPointerB = m_TransformCompManager->GetTransform(object_b);
				ZombieCollisionFeedback(mPointerA, mPointerB, diff);
				ZombieCollisionFeedback(mPointerB, mPointerA, diff);

				if (sphereA.m_CollisionCallback)
				{
					(*sphereA.m_CollisionCallback)({ &sphereA, &sphereB, &mom_a, &mom_b });
				}
				if (sphereB.m_CollisionCallback)
				{
					(*sphereB.m_CollisionCallback)({ &sphereB, &sphereA, &mom_b, &mom_a });
				}
			}
		}
		QuerySphereAgainstAllAABBS(sphereA);

	}
	START_TIME_STAMP(m_DrawDebugLinesTimeStamp);
	DrawDebugLines();
	END_TIME_STAMP(m_DrawDebugLinesTimeStamp);
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
				if (m_TransformCompManager->GetTransform(s.m_GameObject) == nullptr) continue;
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

void CollisionSystem::QuerySphereAgainstAllAABBS(const SphereCollisionComponent& sphere)
{
	if (!GameObjectManager::Instance()->Alive(sphere.m_GameObject))
	{
		return;
	}
	for (int i = 0; i < m_AABBComponentManager->m_Components.Size(); i++)
	{
		AABBCollisionComponent& aabb = m_AABBComponentManager->m_Components[i];
		GameObject aabb_object = aabb.m_GameObject;
		if (!GameObjectManager::Instance()->Alive(aabb_object))
		{
			continue;
		}
		if (m_TransformCompManager->GetTransform(aabb_object) == nullptr) continue;
		ZMasher::Matrix44f transformA = *m_TransformCompManager->GetTransform(sphere.m_GameObject);
		ZMasher::Matrix44f transformB = *m_TransformCompManager->GetTransform(aabb_object);

		float sqDist = 0.f;
		if (SphereVsAABBTest(sphere.m_Radius, aabb.r, transformA.GetTranslation(), transformB.GetTranslation(), sqDist))
		{
			ZMasher::Matrix44f* transformA = m_TransformCompManager->GetTransform(sphere.m_GameObject);
			ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(aabb_object);
			// Sphere and box are intersecting, separate them.
			ZombieCollisionFeedback(transformA, transformB, sqrt(sqDist) - sphere.m_Radius);
		}

	}
}

void CollisionSystem::DrawDebugLines()
{
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
		if(m_TransformCompManager->GetTransform(sphere.m_GameObject) == nullptr)continue;
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
