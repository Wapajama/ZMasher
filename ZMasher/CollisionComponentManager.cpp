#include "CollisionComponentManager.h"



CollisionComponentManager::CollisionComponentManager() 
	: m_Spheres(128)
	, m_Momentums(128)
	, m_Collisions(128)
{
}


CollisionComponentManager::~CollisionComponentManager()
{
}

void CollisionComponentManager::AddComponent(const CollisionType filter, 
											 const float radius, 
											 GameObject game_object,
											 const float weight,
											 const ZMasher::Vector3f& speed)
{
	m_Spheres.Add(SphereCollisionComponent(filter, radius, game_object));
	m_Momentums.Add(MomentumComponent(game_object, weight, speed));
}

bool CollisionComponentManager::Init()
{
	return true;
}

void CollisionComponentManager::Destroy()
{
}

bool CollisionComponentManager::Update()
{
	

	ComponentManager::RemoveDeadComponents(m_Spheres);
	ComponentManager::RemoveDeadComponents(m_Momentums);
	m_Collisions.RemoveAll();//should have been read since last time,
	return true;
}