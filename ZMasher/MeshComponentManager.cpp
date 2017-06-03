#include "MeshComponentManager.h"
#include <ZMasher\TransformComponentManager.h>

MeshComponentManager::MeshComponentManager()
{
}


MeshComponentManager::~MeshComponentManager()
{
}

bool MeshComponentManager::AddComponent(GameObject game_object, ZMModelInstanceNode* instance_node)
{
	m_ModelComponents.Add(ModelComponent(game_object, instance_node));
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
