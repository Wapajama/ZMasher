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
	const int free_index = PopFreeIndex();
	if (free_index == m_FreeIndexes.found_none)
	{
		m_Spheres.Add(SphereCollisionComponent(filter, radius, game_object, callback));
		m_Momentums.Add(MomentumComponent(game_object, weight, speed));
	}
	else
	{
		m_Spheres[free_index] = SphereCollisionComponent(filter, radius, game_object, callback);
		m_Momentums[free_index] = MomentumComponent(game_object, weight, speed);
	}
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
	GAME_OBJECT_KILL(sphere->m_GameObject);
	GAME_OBJECT_KILL(mom->m_GameObject);
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
	return true;
}

MomentumComponent* CollisionComponentManager::GetMomentumComponent(GameObject game_object)
{
	auto comp = m_LookupSet.Find({ game_object, -1 });
	if (comp)
	{
		return &m_Momentums[comp->value.index];
	}
	return nullptr;
}

SphereCollisionComponent* CollisionComponentManager::GetSphereCollisionComponent(GameObject game_object)
{
	auto comp = m_LookupSet.Find({ game_object, -1 });
	if (comp)
	{
		return &m_Spheres[comp->value.index];
	}
	return nullptr;
}
