#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMMatrix44.h>
#include <ZMasher\GameObject.h>

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

class TransformComponentManager :
	public ComponentManager
{
	friend class BulletSystem;
public:
	TransformComponentManager();
	~TransformComponentManager();

	bool Init()override;
	void Destroy()override;
	bool Update()override;

	bool AddComponent(GameObject game_object, const ZMasher::Matrix44f& transform);//will return false if invalid input, eg already exists, invalid transform etc

	ZMasher::Matrix44f* GetTransform(GameObject game_object);//brute force atm, be sure to optimize
	TransformComponent* GetTransformComp(GameObject game_object);

private:
	GrowArray<TransformComponent> m_Transforms;
};

