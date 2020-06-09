#pragma once
#include <Time/Profiler.h>
#include <ZMasher/GameObject.h>
#include <ZMUtils/Math/ZMVector.h>
#include <CollisionDefines.h>
#include <Time/Timer.h>
#include <GlobalIncludes/project_defines.h>

class SphereCollisionComponentManager;
class AABBComponentManager;
class MomentumComponentManager;
class TransformComponentManager;

namespace ZMasher
{
	class Matrix44f;
}

class CollisionSystem
{
public:
	CollisionSystem(SphereCollisionComponentManager* sphere_collision_comp_manager,
					AABBComponentManager* aabb_component_manager,
					MomentumComponentManager* momentum_comp_manager,
					TransformComponentManager* transform_comp_manager);
	~CollisionSystem();

	bool Simulate(const float dt);

	CollisionInfoStruct* GetCollisionInfo(const int index);
	CollisionQuery* CreateQuery(eCOLLISIONTYPE type, GameObject owner, float radius, ZMasher::Vector3f pos);

	CollisionQuery* GetQuery(GameObject owner, int id = 0);

	void Destroy();

private:

	GrowArray<class DebugLineInfo*> m_DebugLines; // buffer of many debuglines to be used for debug rendering
	bool SimulatePhysics(const float dt);
	void DrawDebugLines();
	void DrawAABBs();
	void DrawSpheres();
	void DrawSphere(const float radius, const ZMasher::Vector3f pos);
	void QuerySphereAgainstAllAABBS(const struct SphereCollisionComponent& sphere);
	void ResolveQueries();

	GrowArray<CollisionInfoStruct> m_CollInfos;
	GrowArray<CollisionQuery> m_Queries;
	GrowArray<SphereQueryArgs> m_SphereQueries;
	SphereCollisionComponentManager* const m_SphereCollisionCompManager;
	AABBComponentManager* const m_AABBComponentManager;
	MomentumComponentManager* const m_MomentumCompManager;
	TransformComponentManager* const m_TransformCompManager;

#ifdef BENCHMARK
	Timer m_SingleCollisionTimeStamp;
	Timer m_QueriesTimeStamp;
	Timer m_SimulatePhysicsTimeStamp;
	Timer m_DrawDebugLinesTimeStamp;

	GrowArray<Timer> m_TimeStamps;
#endif // BENCHMARK

};
