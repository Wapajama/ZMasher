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

bool MeshComponentManager::Update(TransformComponentManager* transform_manager)
{
	for (short i = 0; i < m_ModelComponents.Size(); i++)
	{
#ifdef _DEBUG
		assert(transform_manager->GetTransform(m_ModelComponents[i].m_GameObject) && "MeshManager: GameObject doesn't have a transform component!");
#endif // _DEBUG

		m_ModelComponents[i].m_InstanceNode->SetTransform(*transform_manager->GetTransform(m_ModelComponents[i].m_GameObject));
	}
	return true;
}

void MeshComponentManager::Destroy()
{

}
