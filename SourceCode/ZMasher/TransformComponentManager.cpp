#include "TransformComponentManager.h"
#include <ZMasher\GameObjectManager.h>

TransformComponentManager::TransformComponentManager()
{
}


TransformComponentManager::~TransformComponentManager()
{
}

bool TransformComponentManager::Init()
{
	return true;
}

bool TransformComponentManager::Update()
{
	return ComponentManager::Update();
}

TransformComponent* TransformComponentManager::AddComponent(GameObject game_object, const ZMasher::Matrix44f& transform)
{
	ASSERT(GameObjectManager::Instance()->Alive(game_object), "Can't add dead game objects!");
	if (!GameObjectManager::Instance()->Alive(game_object))
	{
		return nullptr;
	}

	ComponentManager::AddComponent({ game_object, transform }, game_object);
	return &m_Components.GetLast();
}

ZMasher::Matrix44f* TransformComponentManager::GetTransform(GameObject game_object)
{
	TransformComponent* comp = GetComponent(game_object);
	if (comp == nullptr)
	{
		return nullptr;
	}
	return &comp->m_Transform;
}

void TransformComponentManager::Destroy()
{
	
}
