#pragma once
#include "ComponentManager.h"
#include <ZMasher\ZMModelInstanceNode.h>
#include <ZMasher\GameObject.h>

class MeshComponentManager :
	public ComponentManager
{
public:
	MeshComponentManager();
	~MeshComponentManager();

	bool Init()override;
	void Destroy()override;
	bool Update(class TransformComponentManager* transform_manager);
	bool AddComponent(GameObject game_object, ZMModelInstanceNode* instance_node);

private:

	struct ModelComponent
	{
		ModelComponent() {}
		ModelComponent(GameObject game_object, ZMModelInstanceNode* instance_node) : 
			m_GameObject(game_object), 
			m_InstanceNode(instance_node){}

		GameObject m_GameObject;
		ZMModelInstanceNode* m_InstanceNode;
	};
	///MemoryContainer<ModelComponent, 1024> m_ModelComponents;//TODO: PUT THIS IN RENDERMANAGER OR SOMETHING, DATA ORIENTED
	GrowArray<ModelComponent> m_ModelComponents;
};
