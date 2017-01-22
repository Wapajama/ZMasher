#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMMatrix44.h>
#include <ZMasher\GameObject.h>

class TransformComponentManager :
	public ComponentManager
{
public:
	TransformComponentManager();
	~TransformComponentManager();

	bool Init()override;
	void Destroy()override;

	bool AddComponent(GameObject game_object, const ZMasher::Matrix44f& transform);//will return false if invalid input, eg already exists, invalid transform etc

	ZMasher::Matrix44f* GetTransform(GameObject game_object);//brute force, be sure to optimize

private:

	struct TransformComponent
	{
		TransformComponent() {}
		TransformComponent(GameObject game_object, const ZMasher::Matrix44f& transform) :
			m_GameObject(game_object),
			m_Transform(transform)//implement move semantics? std::move
		{}
		GameObject m_GameObject;
		ZMasher::Matrix44f m_Transform;
	};

	GrowArray<TransformComponent> m_Transforms;
};

