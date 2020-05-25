#pragma once
#include "ComponentManager.h"
#include <ZMasherGfxDX11\ZMModelInstanceNode.h>
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
	public ComponentManager<ModelComponent>
{
public:
	MeshComponentManager();
	~MeshComponentManager();

	bool Init()override;
	bool Update()override;
	bool Update(class TransformComponentManager* transform_manager);//extract this to a render system instead
	void Destroy()override;
	ModelComponent* AddComponent(GameObject game_object, ZMModelInstanceNode* instance_node);
};
