#pragma once
#include <Time/Profiler.h>
class SphereCollisionComponentManager;
class MomentumComponentManager;
class TransformComponentManager;

class CollisionSystem
{
public:
	CollisionSystem(SphereCollisionComponentManager* sphere_collision_comp_manager,
					MomentumComponentManager* momentum_comp_manager,
					TransformComponentManager* transform_comp_manager);
	~CollisionSystem();

	bool Simulate(const float dt);

private:

	bool SimulatePhysics(const float dt);

	SphereCollisionComponentManager* m_SphereCollisionCompManager;
	MomentumComponentManager* m_MomentumCompManager;
	TransformComponentManager* m_TransformCompManager;
	ProfilerTaskID m_SingleCollisionTimeStamp;
};

