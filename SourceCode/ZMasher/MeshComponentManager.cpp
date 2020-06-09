#include "MeshComponentManager.h"
#include <ZMasher\TransformComponentManager.h>
#include <ZMasher\GameObjectManager.h>

MeshComponentManager::MeshComponentManager()
{
}


MeshComponentManager::~MeshComponentManager()
{
}

//void MeshComponentManager::RemoveComponentWithGameObjectInternal(GameObject object)
//{
//	ModelComponent* mesh = nullptr;
//	int index = -1;
//	for (int i = 0; i < m_Components.Size(); i++)
//	{
//		if (m_Components[i].m_GameObject == object)
//		{
//			mesh = &m_ModelComponents[i];
//			mesh->m_InstanceNode->MarkForDelete();
//			index = i;
//			break;
//		}
//	}
//
//	if (!mesh)
//	{
//		return;
//	}
//	GameObjectManager::Instance()->Destroy(mesh->m_GameObject, false);
//	m_ModelComponents.RemoveCyclic(index);
//}

ModelComponent* MeshComponentManager::AddComponent(GameObject game_object, ZMModelInstanceNode* instance_node)
{
	ComponentManager::AddComponent(ModelComponent(game_object, instance_node), game_object);
	return &m_Components.GetLast();
}

bool MeshComponentManager::Init()
{
	return true;
}

bool MeshComponentManager::Update()
{
	return ComponentManager::Update();
}

bool MeshComponentManager::Update(TransformComponentManager* transform_manager)
{
	for (short i = 0; i < m_Components.Size(); i++)
	{
		TransformComponent* transform_comp = transform_manager->GetComponent(m_Components[i].m_GameObject);
		if (transform_comp == nullptr)
		{
			GameObjectManager::Instance()->Destroy(m_Components[i].m_GameObject);
			m_Components[i].m_InstanceNode->MarkForDelete();
			continue;
		}
		if (!GameObjectManager::Instance()->Alive(m_Components[i].m_GameObject))
		{
			m_Components[i].m_InstanceNode->MarkForDelete();
			continue;
		}
		if (!GameObjectManager::Instance()->Alive(m_Components[i].m_GameObject))
		{
			GameObjectManager::Instance()->Destroy((*transform_comp).m_GameObject);
			m_Components[i].m_InstanceNode->MarkForDelete();
		}
		if (transform_comp != nullptr &&
			!GameObjectManager::Instance()->Alive((*transform_comp).m_GameObject))
		{
			GameObjectManager::Instance()->Destroy(m_Components[i].m_GameObject);
			m_Components[i].m_InstanceNode->MarkForDelete();
		}
#ifdef _DEBUG
		ASSERT(transform_comp, "MeshManager: GameObject doesn't have a transform component!");
#endif // _DEBUG
		m_Components[i].m_InstanceNode->SetTransform(transform_comp->m_Transform);
		
	}
	return true;
}

void MeshComponentManager::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	if (directly)
	{
		if (ModelComponent* mesh = this->GetComponent(object))
		{
			mesh->m_InstanceNode->MarkForDelete();
		}
	}
	this->ComponentManager<ModelComponent>::RemoveComponentWithGameObject(object, directly);
}

void MeshComponentManager::Destroy()
{

}
