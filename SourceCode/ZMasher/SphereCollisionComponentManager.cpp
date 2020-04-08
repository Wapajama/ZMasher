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