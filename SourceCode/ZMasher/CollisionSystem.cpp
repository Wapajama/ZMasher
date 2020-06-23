#include "CollisionSystem.h"
#include <ZMasher\SphereCollisionComponentManager.h>
#include <ZMasher\AABBComponentManager.h>
#include <ZMasher\MomentumComponentManager.h>
#include <ZMasher\TransformComponentManager.h>
#include <ZMasher\GameObjectManager.h>
#include <ZMasherGfxDX11\ZMModelFactory.h>
#include <QuadTree.h>

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
	m_QuadTree = new QuadTree({0,0,0}, AI_RANGE*0.5f);
}

static float SqDistToAABB(const ZMasher::Vector4f& pos_sphere,
						  const float* r,
						  const ZMasher::Vector4f& pos_aabb)
{
	float p[3] = { pos_sphere.x, pos_sphere.y, pos_sphere.z };
	float b[3] = { pos_aabb.x, pos_aabb.y, pos_aabb.z };
	float sqDist = 0.f;
	for (short i = 0; i < 3; i++)
	{
		float v = p[i];
		if (v < (b[i] - r[i])) sqDist += ((b[i] - r[i]) - v) * ((b[i] - r[i]) - v);
		if (v > (b[i] + r[i])) sqDist += (v - (b[i] + r[i])) * (v - (b[i] + r[i]));
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

static bool SphereVsAABBTest(float r_a, float* r_b,
							 const ZMasher::Vector4f& pos_sphere, const ZMasher::Vector4f& pos_aabb, float& sqDistRef)
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

bool CollisionSystem::Init()
{
	m_QuadTree = QuadTree::BuildQuadTree({0,0,0}, AI_RANGE+100);

	return true;
}

void CollisionSystem::AddGameObject(GameObject object)
{
	m_QuadTree->InsertObject(object);
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Destroy()
{
#ifdef BENCHMARK
	Profiler::Instance()->AddTimeStamp(m_SingleCollisionTimeStamp, "SingleCollision");
	Profiler::Instance()->AddTimeStamp(m_QueriesTimeStamp, "Collision Queries");
	Profiler::Instance()->AddTimeStamp(m_SimulatePhysicsTimeStamp, "Simulate Physics");
	Profiler::Instance()->AddTimeStamp(m_DrawDebugLinesTimeStamp, "Draw Debug Lines collisionManager");

	for (int i = 0; i < COLL_N_TIMERS; i++)
	{
		//indexes.Add(("CollisionID: " + std::to_string(i)));
		//Profiler::Instance()->AddTimeStamp(m_TimeStamps[i], indexes.GetLast().c_str());
	}
#endif // BENCHMARK

}
#define START_TIME_STAMP(ts)
#define END_TIME_STAMP(ts)
bool CollisionSystem::Simulate(const float dt)
{
	m_QuadTree->TestCollisions();

	m_CollInfos.RemoveAll();

	START_TIME_STAMP(m_SimulatePhysicsTimeStamp);
	SimulatePhysics(dt);
	END_TIME_STAMP(m_SimulatePhysicsTimeStamp);

	START_TIME_STAMP(m_QueriesTimeStamp);
	ResolveQueries();
	END_TIME_STAMP(m_QueriesTimeStamp);

	//BRUTEFOOOOOOOOOOOOOOOOOOOORCE
	//TODO: Optimize this, might be a major task
	if (false)
	{
		GrowArray<SphereCollisionComponent, int, MAX_GAME_OBJS>& sphereList = m_SphereCollisionCompManager->m_Components;
		GrowArray<float, int, MAX_GAME_OBJS> x;
		GrowArray<float, int, MAX_GAME_OBJS> y;
		GrowArray<float, int, MAX_GAME_OBJS> z;
		GrowArray<GameObject, int, MAX_GAME_OBJS> objs;
		GrowArray<float, int, MAX_GAME_OBJS> rad;

		for (int i = 0; i < sphereList.Size(); i++)
		{
			rad.Add(sphereList[i].m_Radius);
			objs.Add(sphereList[i].m_GameObject);
			if (!m_TransformCompManager->GetTransform(objs.GetLast()))continue;
			ZMasher::Vector4f t = m_TransformCompManager->GetTransform(objs.GetLast())->GetTranslation();
			x.Add(t.x);
			y.Add(t.y);
			z.Add(t.z);
		}
		GrowArray<AABBCollisionComponent, int, MAX_GAME_OBJS>& aabbList = m_AABBComponentManager->m_Components;
		GrowArray<float, int, MAX_GAME_OBJS> aabbx_min;
		GrowArray<float, int, MAX_GAME_OBJS> aabby_min;
		GrowArray<float, int, MAX_GAME_OBJS> aabbz_min;

		GrowArray<float, int, MAX_GAME_OBJS> aabbx_max;
		GrowArray<float, int, MAX_GAME_OBJS> aabby_max;
		GrowArray<float, int, MAX_GAME_OBJS> aabbz_max;

		GrowArray<GameObject, int, MAX_GAME_OBJS> aabbobjs;

		for (int i = 0; i < aabbList.Size(); i++)
		{
			aabbobjs.Add(aabbList[i].m_GameObject);
			ZMasher::Vector4f tran = m_TransformCompManager->GetTranslation(aabbobjs.GetLast());

			aabbx_min.Add(tran.x - aabbList[i].r[0]);
			aabby_min.Add(tran.y - aabbList[i].r[1]);
			aabbz_min.Add(tran.z - aabbList[i].r[2]);

			aabbx_max.Add(tran.x + aabbList[i].r[0]);
			aabby_max.Add(tran.y + aabbList[i].r[1]);
			aabbz_max.Add(tran.z + aabbList[i].r[2]);
		}

		for (int i = 0; i < sphereList.Size(); ++i)
		{
			SphereCollisionComponent sphereA = sphereList[i];

			const GameObject object_a = sphereA.m_GameObject;
			if (!GameObjectManager::Instance()->Alive(object_a))
			{
				continue;
			}
			const ZMasher::Matrix44f transformA = *m_TransformCompManager->GetTransform(object_a);
			const ZMasher::Vector4f posA = transformA.GetTranslation();
			const float rA = sphereA.m_Radius;
			const float posAx = posA.x;
			const float posAy = posA.y;
			const float posAz = posA.z;

			for (int j = i + 1; j + 4 < sphereList.Size(); j += 4)
			{
				float t1[4] = { posAx - x[j],
								posAx - x[j + 1],
								posAx - x[j + 2],
								posAx - x[j + 3] };

				t1[0] = t1[0] * t1[0];
				t1[1] = t1[1] * t1[1];
				t1[2] = t1[2] * t1[2];
				t1[3] = t1[3] * t1[3];

				float t2[4] = { posAy - y[j],
								posAy - y[j + 1],
								posAy - y[j + 2],
								posAy - y[j + 3] };

				t2[0] = t2[0] * t2[0];
				t2[1] = t2[1] * t2[1];
				t2[2] = t2[2] * t2[2];
				t2[3] = t2[3] * t2[3];

				float t3[4] = { posAz - z[j],
								posAz - z[j + 1],
								posAz - z[j + 2],
								posAz - z[j + 3] };

				t3[0] = t3[0] * t3[0];
				t3[1] = t3[1] * t3[1];
				t3[2] = t3[2] * t3[2];
				t3[3] = t3[3] * t3[3];

				t1[0] = t1[0] + t2[0];
				t1[1] = t1[1] + t2[1];
				t1[2] = t1[2] + t2[2];
				t1[3] = t1[3] + t2[3];

				float t4[4] = { rA + rad[j],
								rA + rad[j + 1],
								rA + rad[j + 2],
								rA + rad[j + 3] };

				t4[0] = t4[0] * t4[0];
				t4[1] = t4[1] * t4[1];
				t4[2] = t4[2] * t4[2];
				t4[3] = t4[3] * t4[3];

				t2[0] = t4[0] - t3[0];
				t2[1] = t4[1] - t3[1];
				t2[2] = t4[2] - t3[2];
				t2[3] = t4[3] - t3[3];

				bool r[4] = { t1[0] <= t2[0],
								t1[1] <= t2[1],
								t1[2] <= t2[2],
								t1[3] <= t2[3] };

				if (r[0])
				{
					SphereCollision(object_a, objs[j], sphereA, *(m_SphereCollisionCompManager->GetComponent(objs[j])));
				}
				if (r[1])
				{
					SphereCollision(object_a, objs[j + 1], sphereA, *(m_SphereCollisionCompManager->GetComponent(objs[j + 1])));
				}
				if (r[2])
				{
					SphereCollision(object_a, objs[j + 2], sphereA, *(m_SphereCollisionCompManager->GetComponent(objs[j + 2])));
				}
				if (r[3])
				{
					SphereCollision(object_a, objs[j + 3], sphereA, *(m_SphereCollisionCompManager->GetComponent(objs[j + 3])));
				}
				//else
				//{
				//	QuerySphereAgainstSphere(j, object_a, sphereA);
				//}
				if (!GameObjectManager::Instance()->Alive(object_a))
				{
					break;
				}
			}

			//QuerySphereAgainstAllAABBS(sphereA);

			for (int j = 0; j < aabbobjs.Size(); j += 4)
			{
#pragma region SIMDAABB
				float minx[4] = {
				aabbx_min[j],
				aabbx_min[j + 1],
				aabbx_min[j + 2],
				aabbx_min[j + 3] };

				float tx[4] = {
					max(minx[0], posAx),
					max(minx[1], posAx),
					max(minx[2], posAx),
					max(minx[3], posAx) };

				float miny[4] = {
					aabby_min[j],
					aabby_min[j + 1],
					aabby_min[j + 2],
					aabby_min[j + 3] };

				float ty[4] = {
					max(miny[0], posAy),
					max(miny[1], posAy),
					max(miny[2], posAy),
					max(miny[3], posAy) };

				float minz[4] = {
					aabbz_min[j],
					aabbz_min[j + 1],
					aabbz_min[j + 2],
					aabbz_min[j + 3] };

				float tz[4] = {
					max(minz[0], posAz),
					max(minz[1], posAz),
					max(minz[2], posAz),
					max(minz[3], posAz) };

				float maxx[4] = {
					aabbx_max[j],
					aabbx_max[j + 1],
					aabbx_max[j + 2],
					aabbx_max[j + 3] };

				tx[0] = min(maxx[0], tx[0]);
				tx[1] = min(maxx[1], tx[1]);
				tx[2] = min(maxx[2], tx[2]);
				tx[3] = min(maxx[3], tx[3]);

				float dx[4] = {
					posAx - tx[0],
					posAx - tx[1],
					posAx - tx[2],
					posAx - tx[3] };

				dx[0] = dx[0] * dx[0];
				dx[1] = dx[1] * dx[1];
				dx[2] = dx[2] * dx[2];
				dx[3] = dx[3] * dx[3];

				float maxy[4] = {
					aabby_max[j],
					aabby_max[j + 1],
					aabby_max[j + 2],
					aabby_max[j + 3] };

				ty[0] = min(maxy[0], ty[0]);
				ty[1] = min(maxy[1], ty[1]);
				ty[2] = min(maxy[2], ty[2]);
				ty[3] = min(maxy[3], ty[3]);

				float dy[4] = {
					posAy - ty[0],
					posAy - ty[1],
					posAy - ty[2],
					posAy - ty[3] };

				dy[0] = dy[0] * dy[0];
				dy[1] = dy[1] * dy[1];
				dy[2] = dy[2] * dy[2];
				dy[3] = dy[3] * dy[3];

				float maxz[4] = {
					aabbz_max[j],
					aabbz_max[j + 1],
					aabbz_max[j + 2],
					aabbz_max[j + 3] };

				tz[0] = min(maxz[0], tz[0]);
				tz[1] = min(maxz[1], tz[1]);
				tz[2] = min(maxz[2], tz[2]);
				tz[3] = min(maxz[3], tz[3]);

				float dz[4] = {
					posAz - tz[0],
					posAz - tz[1],
					posAz - tz[2],
					posAz - tz[3] };

				dz[0] = dz[0] * dz[0];
				dz[1] = dz[1] * dz[1];
				dz[2] = dz[2] * dz[2];
				dz[3] = dz[3] * dz[3];

				float r2[4] = {
					rA * rA,
					rA * rA,
					rA * rA,
					rA * rA };

				float t1[4] = {
					dx[0] + dy[0],
					dx[1] + dy[1],
					dx[2] + dy[2],
					dx[3] + dy[3]
				};

				float t2[4] = {
					r2[0] - dz[0],
					r2[1] - dz[1],
					r2[2] - dz[2],
					r2[3] - dz[3]
				};

				bool result[4] = {
					t1[0] <= t2[0],
					t1[1] <= t2[1],
					t1[2] <= t2[2],
					t1[3] <= t2[3]
				};
#pragma endregion

				for (int index = 0; index < 4; index++)
				{
					if (result[index])
					{
						ZMasher::Matrix44f* transform_A = m_TransformCompManager->GetTransform(sphereA.m_GameObject);
						ZMasher::Matrix44f* transform_B = m_TransformCompManager->GetTransform(aabbobjs[j + index]);
						// Sphere and box are intersecting, separate them.
						AABBCollisionComponent* aabb = m_AABBComponentManager->GetComponent(aabbobjs[j + index]);
						const float sqDist = SqDistToAABB(transform_A->GetTranslation(), aabb->r, transform_B->GetTranslation());
						ZombieCollisionFeedback(transform_A, transform_B, sqrt(sqDist) - rA);
						if ((*sphereA.m_CollisionCallback))
						{
							(*sphereA.m_CollisionCallback)({ &sphereA, nullptr, nullptr, nullptr, nullptr, nullptr });
						}
					}
				}
				if (!GameObjectManager::Instance()->Alive(object_a))
				{
					break;
				}
			}
		}
	}

	START_TIME_STAMP(m_DrawDebugLinesTimeStamp);
	DrawDebugLines();
	END_TIME_STAMP(m_DrawDebugLinesTimeStamp);
	return true;
}

void CollisionSystem::QuerySphereAgainstSphere(int j,
											   const GameObject & __restrict object_a, 
											   SphereCollisionComponent & __restrict sphereA)
{
	const SphereCollisionComponent sphereB = m_SphereCollisionCompManager->m_Components[j];
	const GameObject object_b = sphereB.m_GameObject;
	if (!GameObjectManager::Instance()->Alive(object_b))
	{
		 return;
	}
	const bool collisionResult = SphereVsSphereTest(sphereA.m_Radius,
													sphereB.m_Radius,
													m_TransformCompManager->GetTranslation(object_a), 
													m_TransformCompManager->GetTranslation(object_b));
	if (collisionResult)
	{
		SphereCollision(object_a, object_b, sphereA, sphereB);
	}
}

void CollisionSystem::SphereCollision(GameObject object_a, GameObject object_b, const SphereCollisionComponent& sphereA, const SphereCollisionComponent& sphereB)
{
	if (!GameObjectManager::Instance()->Alive(object_a)||
		!GameObjectManager::Instance()->Alive(object_b))
	{
		return;
	}
	const ZMasher::Matrix44f transformA = *m_TransformCompManager->GetTransform(object_a);
	const ZMasher::Matrix44f transformB = *m_TransformCompManager->GetTransform(object_b);

	const MomentumComponent mom_a = *m_MomentumCompManager->GetComponent(object_a);
	const MomentumComponent mom_b = *m_MomentumCompManager->GetComponent(object_b);

	ZMasher::Vector3f diffv = transformA.GetTranslation().ToVector3f() - transformB.GetTranslation().ToVector3f();
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
		if (m_TransformCompManager->GetTransform(sphere.m_GameObject) == nullptr)continue;
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
	ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(radius, 0, 0), pos + ZMasher::Vector3f(-radius, 0, 0), eColour::RED);
	ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(0, radius, 0), pos + ZMasher::Vector3f(0, -radius, 0), eColour::RED);
	ZMModelFactory::Instance()->CreateDebugLine(pos + ZMasher::Vector3f(0, 0, radius), pos + ZMasher::Vector3f(0, 0, -radius), eColour::RED);
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
