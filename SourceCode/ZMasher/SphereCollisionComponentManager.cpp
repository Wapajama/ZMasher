#include "SphereCollisionComponentManager.h"
#include <ZMasher\GameObjectManager.h>


SphereCollisionComponentManager::SphereCollisionComponentManager() 
{
}


SphereCollisionComponentManager::~SphereCollisionComponentManager()
{
}

SphereCollisionComponent* SphereCollisionComponentManager::AddComponent(const eCOLLISIONTYPE filter,
												const float radius, 
												GameObject game_object,
												CollisionCallBack callback)
{
	ComponentManager::AddComponent({ filter, radius, game_object, callback }, game_object);
	return &m_Components.GetLast();
}

void SphereCollisionComponentManager::DeleteComponentCallback(GameObject component)
{
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