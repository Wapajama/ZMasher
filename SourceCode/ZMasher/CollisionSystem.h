#pragma once
#include <Time/Profiler.h>
#include <ZMasher/GameObject.h>
#include <ZMUtils/Math/ZMVector.h>
class SphereCollisionComponentManager;
class AABBComponentManager;
class MomentumComponentManager;
class TransformComponentManager;

struct CollisionInfoStruct
{
	GameObject objectA;
	GameObject objectB;
	ZMasher::Vector3f posA;
	ZMasher::Vector3f posB;
};

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

private:


	GrowArray<class DebugLineInfo*> m_DebugLines; // buffer of many debuglines to be used for debug rendering
	bool SimulatePhysics(const float dt);
	void DrawDebugLines();
	void DrawAABBs();
	void DrawSpheres();
	void QuerySphereAgainstAllAABBS(const struct SphereCollisionComponent& sphere, ZMasher::Matrix44f* transform);
	GrowArray<CollisionInfoStruct> m_CollInfos;
	SphereCollisionComponentManager* m_SphereCollisionCompManager;
	AABBComponentManager* m_AABBComponentManager;
	MomentumComponentManager* m_MomentumCompManager;
	TransformComponentManager* m_TransformCompManager;
	ProfilerTaskID m_SingleCollisionTimeStamp;
};

