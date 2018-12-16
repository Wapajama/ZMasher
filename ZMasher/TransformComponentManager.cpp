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

bool TransformComponentManager::AddComponent(GameObject game_object, const ZMasher::Matrix44f& transform)
{
	ASSERT(GameObjectManager::Instance()->Alive(game_object), "Can't add dead game objects!");
	// TODO: ADD FUNCTIONALITY FOR REPLACING DEAD COMPONENTS, INSTEAD OF REMOVING THEM
	//m_Components.Add({ game_object, transform });
	//m_LookupSet.Insert({game_object, m_Components.Size()-1});

	ComponentManager::AddComponent({ game_object, transform }, game_object);
	return true;
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
