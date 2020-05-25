#pragma once
#include "ComponentManager.h"
#include <Math\ZMVector3.h>

struct MomentumComponent //no need for gameobject id, they are aligned in the same order
{
	GameObject m_GameObject;
	float m_Weight;
	ZMasher::Vector3f m_Speed;
	ZMasher::Vector3f m_Acceleration;
};


class MomentumComponentManager :
	public ComponentManager<MomentumComponent>
{
public:
	MomentumComponentManager();
	~MomentumComponentManager();

	MomentumComponent* AddComponent(
		GameObject game_object,
		const float weight,
		const ZMasher::Vector3f& speed = ZMasher::Vector3f());

	bool Init()override { return true; }
	void Destroy()override {}
	bool Update()override { return true; }

	friend class CollisionSystem;
};

