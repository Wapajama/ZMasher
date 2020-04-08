#include "AABBComponentManager.h"

AABBComponentManager::AABBComponentManager()
{
}


AABBComponentManager::~AABBComponentManager()
{
}

void AABBComponentManager::AddComponent(const CollisionType filter,
	const float halfwidth[3],
	GameObject game_object,
	CollisionCallBack callback)
{
	ComponentManager::AddComponent({halfwidth[0], halfwidth[1], halfwidth[2],
		filter, game_object, callback}, game_object);
}

bool AABBComponentManager::Init()
{
	return true;
}

void AABBComponentManager::Destroy()
{

}

bool AABBComponentManager::Update()
{
	return ComponentManager::Update();
}
