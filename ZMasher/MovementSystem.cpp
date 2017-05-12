#include "MovementSystem.h"
#include <ZMasher\CollisionComponentManager.h>
#include <ZMasher\TransformComponentManager.h>


MovementSystem::MovementSystem(CollisionComponentManager* collision,
							   TransformComponentManager* transform)
	: m_CollisionMngr(collision)
	, m_TransformMngr(transform)
{
}


MovementSystem::~MovementSystem()
{
}

bool MovementSystem::Update(const float dt)
{
	int* derp;
	void* derp2;

	static_cast<void>(derp);


	return true;
}