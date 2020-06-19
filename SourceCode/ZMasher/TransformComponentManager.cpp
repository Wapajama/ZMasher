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
	for (int i = 0; i < MAX_GAME_OBJS; i++)
	{
		//m_Translations.Add(ZMasher::Vector4f());
		//m_TranslationsX.Add(FLT_MAX);
		//m_TranslationsY.Add(FLT_MAX);
		//m_TranslationsZ.Add(FLT_MAX);
	}
	return true;
}

bool TransformComponentManager::Update()
{
	for (int i = 0; i < m_Components.Size(); i++)
	{
		this->GetTranslation(m_Components[i].m_GameObject) = m_Components[i].m_Transform.GetTranslation();
	}
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
