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
	for (short i = 0; i < m_Transforms.Size(); ++i)
	{
		if (m_Transforms[i].m_GameObject == game_object)
		{
			return &m_Transforms[i];
		}
	}
	return nullptr;
}

void TransformComponentManager::Destroy()
{
	
}
