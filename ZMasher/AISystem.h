#pragma once
#include <ZMasher\BaseSystem.h>


class AISystem
	:public BaseSystem
{
public:
	AISystem(class AIComponentManager* ai,
			 class CollisionComponentManager* collision,
			 class TransformComponentManager* transform);
	~AISystem();

	bool Simulate(const float dt);

private:

	AIComponentManager* m_AIMngr;
	CollisionComponentManager* m_CollisionMngr;
	TransformComponentManager* m_TransformMngr;

};

