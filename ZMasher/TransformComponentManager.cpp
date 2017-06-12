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
	m_Transforms.Add(TransformComponent(game_object, transform));
	m_LookupSet.Insert({game_object, m_Transforms.Size()-1});
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
	return &m_Transforms[m_LookupSet.Find({game_object, -1})->value.index];
}

void TransformComponentManager::Destroy()
{
	
}
