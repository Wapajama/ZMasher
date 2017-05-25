#pragma once

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

	CollisionComponentManager* m_CollisionCompManager;
	TransformComponentManager* m_TransformCompManager;
};
