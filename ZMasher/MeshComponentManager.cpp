#include "MeshComponentManager.h"
#include <ZMasher\TransformComponentManager.h>

MeshComponentManager::MeshComponentManager()
{
}


MeshComponentManager::~MeshComponentManager()
{
}

void MeshComponentManager::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	ModelComponent* mesh = nullptr;
	for (int i = 0; i < m_ModelComponents.Size(); i++)
	{
		if (m_ModelComponents[i].m_GameObject == object)
		{
			mesh = &m_ModelComponents[i];
			break;
		}
	}

	if (!mesh)
	{
		return;
	}
	if (GAME_OBJECT_IS_ALIVE(mesh->m_GameObject))
	{
		GAME_OBJECT_TOGGLE_ALIVE_GO(mesh->m_GameObject);
	}
}

bool MeshComponentManager::AddComponent(GameObject game_object, ZMModelInstanceNode* instance_node)
{
	const int free_index = PopFreeIndex();
	if (free_index == m_FreeIndexes.found_none)
	{
		m_ModelComponents.Add(ModelComponent(game_object, instance_node));
	}
	else
	{
		m_ModelComponents[free_index] = ModelComponent(game_object, instance_node);
	}
	return true;
}

bool MeshComponentManager::Init()
{

	return true;
}

bool MeshComponentManager::Update()
{
	RemoveDeadComponents(m_ModelComponents);
	return true;
}

bool MeshComponentManager::Update(TransformComponentManager* transform_manager)
{
	for (short i = 0; i < m_ModelComponents.Size(); i++)
	{
		TransformComponent* transform_comp = transform_manager->GetTransformComp(m_ModelComponents[i].m_GameObject);
		if (transform_comp == nullptr)
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(m_ModelComponents[i].m_GameObject);
			continue;
		}
		if (m_ModelComponents[i].m_GameObject == NULL_GAME_OBJECT ||
			!GAME_OBJECT_IS_ALIVE( m_ModelComponents[i].m_GameObject))
		{
			continue;
		}
		if (!GAME_OBJECT_IS_ALIVE(m_ModelComponents[i].m_GameObject))
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO((*transform_comp).m_GameObject);
		}
		if (transform_comp != nullptr &&
			!GAME_OBJECT_IS_ALIVE((*transform_comp).m_GameObject))
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(m_ModelComponents[i].m_GameObject);
			m_ModelComponents[i].m_InstanceNode->MarkForDelete();
		}
#ifdef _DEBUG
		ASSERT(transform_comp, "MeshManager: GameObject doesn't have a transform component!");
#endif // _DEBUG
		m_ModelComponents[i].m_InstanceNode->SetTransform(transform_comp->m_Transform);
		
	}
	return true;
}

void MeshComponentManager::Destroy()
{

}
