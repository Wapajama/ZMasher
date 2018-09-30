#include "CollisionComponentManager.h"



CollisionComponentManager::CollisionComponentManager() 
{
}


CollisionComponentManager::~CollisionComponentManager()
{
}

void CollisionComponentManager::AddComponent(	const CollisionType filter, 
												const float radius, 
												GameObject game_object,
												const float weight,
												CollisionCallBack callback,
												const ZMasher::Vector3f& speed)
{
	m_Spheres.Add(SphereCollisionComponent(filter, radius, game_object, callback));
	m_Momentums.Add(MomentumComponent(game_object, weight, speed));
	m_LookupSet.Insert({game_object, m_Spheres.Size()-1});
}

void CollisionComponentManager::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	SphereCollisionComponent* sphere = this->GetSphereCollisionComponent(object);
	MomentumComponent* mom = this->GetMomentumComponent(object);
	if (!sphere)
	{
		ASSERT(mom == nullptr, "collision pair only has a momentumcomponent!");
		return;
	}
	ASSERT(mom != nullptr, "collision pair only has a collisioncomponent!");
	if (GAME_OBJECT_IS_ALIVE(sphere->m_GameObject))
	{
		GAME_OBJECT_TOGGLE_ALIVE_GO(sphere->m_GameObject);
	}
	if (GAME_OBJECT_IS_ALIVE(mom->m_GameObject))
	{
		GAME_OBJECT_TOGGLE_ALIVE_GO(mom->m_GameObject);
	}
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
	m_Collisions.RemoveAll();//should have been read since last time
	return true;
}

MomentumComponent* CollisionComponentManager::GetMomentumComponent(GameObject game_object)
{
	return &m_Momentums[m_LookupSet.Find({game_object, -1})->value.index];
}

SphereCollisionComponent* CollisionComponentManager::GetSphereCollisionComponent(GameObject game_object)
{
	return &m_Spheres[m_LookupSet.Find({game_object, -1})->value.index];
}
