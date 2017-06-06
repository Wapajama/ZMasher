#pragma once
#include "ComponentManager.h"
#include <ZMasher\ZMModelInstanceNode.h>
#include <ZMasher\GameObject.h>

struct ModelComponent
{
	ModelComponent() {}
	ModelComponent(GameObject game_object, ZMModelInstanceNode* instance_node) :
		m_GameObject(game_object),
		m_InstanceNode(instance_node) {}

	GameObject m_GameObject;
	ZMModelInstanceNode* m_InstanceNode;
};

class MeshComponentManager :
	public ComponentManager
{
public:
	MeshComponentManager();
	~MeshComponentManager();

	bool Init()override;
	bool Update()override;
	bool Update(class TransformComponentManager* transform_manager);//extract this to a render system instead
	bool AddComponent(GameObject game_object, ZMModelInstanceNode* instance_node);
	void Destroy()override;

private:

	GrowArray<ModelComponent> m_ModelComponents;
};
