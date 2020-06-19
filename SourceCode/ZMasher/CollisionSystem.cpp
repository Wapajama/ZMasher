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

bool SphereVs4Spheres(float r_x, ZMasher::Vector4f pos_x,
									float r_a, ZMasher::Vector4f pos_a,
									float r_b, ZMasher::Vector4f pos_b,
									float r_c, ZMasher::Vector4f pos_c,
									float r_d, ZMasher::Vector4f pos_d,
									bool& __restrict a, bool& __restrict b, bool& __restrict c, bool& __restrict d)
{
	//const __m128 xx = { pos_x.x, pos_x.x, pos_x.x, pos_x.x };
	//const __m128 xy = { pos_x.y, pos_x.y, pos_x.y, pos_x.y };
	//const __m128 xz = { pos_x.z, pos_x.z, pos_x.z, pos_x.z };
	//const __m128 xr = { r_x, r_x, r_x, r_x };


	//const __m128 qx = { pos_a.x, pos_b.x, pos_c.x, pos_d.x };
	//const __m128 qy = { pos_a.y, pos_b.y, pos_c.y, pos_d.y };
	//const __m128 qz = { pos_a.z, pos_b.z, pos_c.z, pos_d.z };
	//const __m128 qr = { r_a, r_b, r_c, r_d					};
	//__m128 t1 = _mm_sub_ps(xx, qx);
	//__m128 t2 = _mm_sub_ps(xy, qy);
	//__m128 t3 = _mm_sub_ps(xz, qz);
	//__m128 t4 = _mm_add_ps(xr, qr);
	//const __m128 r2 = _mm_mul_ps(t4,t4);

	//t1 = _mm_mul_ps(t1,t1);
	//t2 = _mm_mul_ps(t2,t2);
	//t3 = _mm_mul_ps(t3,t3);

	//t1 = _mm_add_ps(t1, t2);
	//t2 = _mm_sub_ps(r2, t3);

	//const __m128 res = _mm_cmple_ps(t1, t2);

	//if (!res.m128_i64[0] &&
	//	!res.m128_i64[1])
	//{
	//	return false;
	//}

	//a = static_cast<bool>(res.m128_i32[0]);
	//b = static_cast<bool>(res.m128_i32[1]);
	//c = static_cast<bool>(res.m128_i32[2]);
	//d = static_cast<bool>(res.m128_i32[3]);

	return a || b || c || d;
}
	//__m128 xx = _mm_set_ps(pos_x.x, pos_x.x, pos_x.x, pos_x.x);
	//__m128 xy = _mm_set_ps(pos_x.y, pos_x.y, pos_x.y, pos_x.y);
	//__m128 xz = _mm_set_ps(pos_x.z, pos_x.z, pos_x.z, pos_x.z);
	//__m128 xr = _mm_set_ps( r_x, r_x, r_x, r_x );

	//__m128 qx = _mm_set_ps( pos_a.x, pos_b.x, pos_c.x, pos_d.x);
	//__m128 qy = _mm_set_ps( pos_a.y, pos_b.y, pos_c.y, pos_d.y);
	//__m128 qz = _mm_set_ps( pos_a.z, pos_b.z, pos_c.z, pos_d.z);
	//__m128 qr = _mm_set_ps( r_a, r_b, r_c, r_d);


	//__m128 t1 = _mm_sub_ps({ pos_x.x, pos_x.x, pos_x.x, pos_x.x }, { pos_a.x, pos_b.x, pos_c.x, pos_d.x });
	//__m128 t2 = _mm_sub_ps({ pos_x.y, pos_x.y, pos_x.y, pos_x.y }, { pos_a.y, pos_b.y, pos_c.y, pos_d.y });
	//__m128 t3 = _mm_sub_ps({ pos_x.z, pos_x.z, pos_x.z, pos_x.z }, { pos_a.z, pos_b.z, pos_c.z, pos_d.z });
	//__m128 t4 = _mm_add_ps({ r_x, r_x, r_x, r_x }, qr);

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
	const int compSize = m_SphereCollisionCompManager->m_Components.Size();
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
		
		for (int j = i + 1; j + 4 < sphereList.Size(); ++j)
		{
			// Query sphere against 4 spheres
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

				bool r[4] = {	t1[0] <= t2[0],
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

				j+=3;
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
		
		for (int j = 0; j < m_AABBComponentManager->m_Components.Size(); j+=4)
		{
			float minx[4] = {	
				aabbx_min[j], 
				aabbx_min[j+1],
				aabbx_min[j+2],
				aabbx_min[j+3]};

			float tx[4] = {
				max(minx[0], posAx), 
				max(minx[1], posAx),
				max(minx[2], posAx),
				max(minx[3], posAx)};

			float miny[4] = {
				aabby_min[j], 
				aabby_min[j+1],
				aabby_min[j+2],
				aabby_min[j+3]};

			float ty[4] = {
				max(miny[0], posAy),
				max(miny[1], posAy),
				max(miny[2], posAy),
				max(miny[3], posAy)};

			float minz[4] = {
				aabby_min[j], 
				aabby_min[j+1],
				aabby_min[j+2],
				aabby_min[j+3]};

			float tz[4] = {
				max(minz[0], posAz),
				max(minz[1], posAz),
				max(minz[2], posAz),
				max(minz[3], posAz)};

			float maxx[4] = {
				aabbx_max[j], 
				aabbx_max[j+1],
				aabbx_max[j+2],
				aabbx_max[j+3]};

			tx[0] = min(maxx[0], posAx);
			tx[1] = min(maxx[1], posAx);
			tx[2] = min(maxx[2], posAx);
			tx[3] = min(maxx[3], posAx);

			float dx[4] = {
				posAx - tx[0],
				posAx - tx[1],
				posAx - tx[2],
				posAx - tx[3]};

			dx[0] = dx[0] * dx[0];
			dx[1] = dx[1] * dx[1];
			dx[2] = dx[2] * dx[2];
			dx[3] = dx[3] * dx[3];

			float maxy[4] = {
				aabby_max[j], 
				aabby_max[j+1],
				aabby_max[j+2],
				aabby_max[j+3]};

			ty[0] = min(maxy[0], posAy);
			ty[1] = min(maxy[1], posAy);
			ty[2] = min(maxy[2], posAy);
			ty[3] = min(maxy[3], posAy);

			float dy[4] = {
				posAy - ty[0],
				posAy - ty[1],
				posAy - ty[2],
				posAy - ty[3]};

			dy[0] = dy[0] * dy[0];
			dy[1] = dy[1] * dy[1];
			dy[2] = dy[2] * dy[2];
			dy[3] = dy[3] * dy[3];

			float maxz[4] = {
				aabbz_max[j], 
				aabbz_max[j+1],
				aabbz_max[j+2],
				aabbz_max[j+3]};

			tz[0] = min(maxz[0], posAz);
			tz[1] = min(maxz[1], posAz);
			tz[2] = min(maxz[2], posAz);
			tz[3] = min(maxz[3], posAz);

			float dz[4] = {
				posAz - tz[0],
				posAz - tz[1],
				posAz - tz[2],
				posAz - tz[3]};

			dz[0] = dz[0] * dz[0];
			dz[1] = dz[1] * dz[1];
			dz[2] = dz[2] * dz[2];
			dz[3] = dz[3] * dz[3];

			float r2[4] = {
				rA * rA,
				rA * rA,
				rA * rA,
				rA * rA};

			float t1[4] = {
				dx[0] + dy[0],
				dx[1] + dy[1],
				dx[2] + dy[2],
				dx[3] + dy[3]
			};

			float t2[4] = {
				r2[0] - dz[0],
				r2[0] - dz[1],
				r2[0] - dz[2],
				r2[0] - dz[3]
			};

			bool result[4] = {
				t1[0] <= t2[0],
				t1[1] <= t2[1],
				t1[2] <= t2[2],
				t1[3] <= t2[3]
			};
			
			for (byte index = 0; i < 4; i++)
			{
				if (result[index])
				{
					ZMasher::Matrix44f* transformA = m_TransformCompManager->GetTransform(sphereA.m_GameObject);
					ZMasher::Matrix44f* transformB = m_TransformCompManager->GetTransform(aabbobjs[index]);
					// Sphere and box are intersecting, separate them.
					float r = (aabbx_min[index] - aabbx_max[index]) * 0.5f;
					const float sqDist = SqDistToAABB(transformA->GetTranslation(), &r, transformB->GetTranslation());
					ZombieCollisionFeedback(transformA, transformB, sqrt(sqDist) - rA);
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
