#include "SphereCollisionComponentManager.h"
#include <ZMasher\GameObjectManager.h>


SphereCollisionComponentManager::SphereCollisionComponentManager() 
{
}


SphereCollisionComponentManager::~SphereCollisionComponentManager()
{
}

void SphereCollisionComponentManager::AddComponent(	const CollisionType filter, 
												const float radius, 
												GameObject game_object,
												CollisionCallBack callback)
{
	ComponentManager::AddComponent({ filter, radius, game_object, callback }, game_object);
}

bool SphereCollisionComponentManager::Init()
{
	return true;
}

void SphereCollisionComponentManager::Destroy()
{

}

bool SphereCollisionComponentManager::Update()
{	
	return ComponentManager::Update();
}

//MomentumComponent* SphereCollisionComponentManager::GetMomentumComponent(GameObject game_object)
//{
//	auto comp = m_LookupSet.Find({ game_object, -1 });
//	if (comp)
//	{
//		return &m_Momentums[comp->value.index];
//	}
//	return nullptr;
//}
//
//SphereCollisionComponent* SphereCollisionComponentManager::GetSphereCollisionComponent(GameObject game_object)
//{
//	auto comp = m_LookupSet.Find({ game_object, -1 });
//	if (comp)
//	{
//		return &m_Spheres[comp->value.index];
//	}
//	return nullptr;
//}

//void SphereCollisionComponentManager::RemoveComponentWithGameObjectInternal(GameObject object)
//{
//	SphereCollisionComponent* sphere = this->GetSphereCollisionComponent(object);
//	MomentumComponent* mom = this->GetMomentumComponent(object);
//	if (sphere == nullptr &&
//		mom == nullptr)
//	{
//		return;
//	}
//	if (!sphere)
//	{
//		ASSERT(mom == nullptr, "collision pair only has a momentumcomponent!");
//		return;
//	}
//	ASSERT(mom != nullptr, "collision pair only has a collisioncomponent!");
//	GameObjectManager::Instance()->Destroy(sphere->m_GameObject, false);
//	GameObjectManager::Instance()->Destroy(mom->m_GameObject, false);
//	
//	auto comp = m_LookupSet.Find({ object, -1 });
//	if (comp != nullptr)
//	{
//		// both components have the same index
//		m_Spheres.RemoveCyclic(comp->value.index);
//		m_Momentums.RemoveCyclic(comp->value.index);
//
//		if (comp->value.index < m_Spheres.Size() &&
//			m_Spheres.Size() > 0)
//		{
//			GameObject new_game_object = m_Spheres[comp->value.index].m_GameObject;
//			auto new_comp = m_LookupSet.Find({ new_game_object, -1 });
//			new_comp->value.index = comp->value.index;
//		}
//
//		m_LookupSet.Delete({ object,-1 });
//	}
//
//}
