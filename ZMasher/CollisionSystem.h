#pragma once
#include <Time/Profiler.h>
class CollisionComponentManager;
class TransformComponentManager;

class CollisionSystem
{
public:
	CollisionSystem(CollisionComponentManager* collision_comp_manager,
					TransformComponentManager* transform_comp_manager);
	~CollisionSystem();

	bool Simulate(const float dt);

private:

	bool SimulatePhysics(const float dt);

	CollisionComponentManager* m_CollisionCompManager;
	TransformComponentManager* m_TransformCompManager;
	ProfilerTaskID m_SingleCollisionTimeStamp;
};

