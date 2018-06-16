#include "TransformComponentManager.h"

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
	RemoveDeadComponents(m_Transforms);
	return true;
}

bool TransformComponentManager::AddComponent(GameObject game_object, const ZMasher::Matrix44f& transform)
{
	ASSERT(GAME_OBJECT_IS_ALIVE(game_object), "Can't add dead game objects!");
	// TODO: ADD FUNCTIONALITY FOR REPLACING DEAD COMPONENTS, INSTEAD OF REMOVING THEM
	const int free_index = PopFreeIndex();
	if (free_index == m_FreeIndexes.found_none)
	{
		m_Transforms.Add(TransformComponent(game_object, transform));
	}
	else
	{
		m_Transforms[free_index] = TransformComponent(game_object, transform);
	}
	m_LookupSet.Insert({game_object, free_index != m_FreeIndexes.found_none ? free_index : m_Transforms.Size()-1});
	return true;
}

ZMasher::Matrix44f* TransformComponentManager::GetTransform(GameObject game_object)
{
	TransformComponent* comp = GetTransformComp(game_object);
	if (comp == nullptr)
	{
		return nullptr;
	}
	return &comp->m_Transform;
}

TransformComponent* TransformComponentManager::GetTransformComp(GameObject game_object)
{
	if (m_LookupSet.Find({ game_object, -1 }) == nullptr)
	{
		return nullptr;
	}
	return &m_Transforms[m_LookupSet.Find({game_object, -1})->value.index];
}

void TransformComponentManager::PostRemoveDeadComponents(GameObject game_object)
{
	// it was added as alive, so set the index to alive again to be able to find it
	if (!GAME_OBJECT_IS_ALIVE(game_object))
	{
		GAME_OBJECT_TOGGLE_ALIVE_GO(game_object);
	}
	m_LookupSet.Delete({ game_object, -1 });
}

void TransformComponentManager::Destroy()
{
	
}
