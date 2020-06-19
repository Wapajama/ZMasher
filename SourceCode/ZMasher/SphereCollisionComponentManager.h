#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMVector3.h>
#include <DataStructures\BinarySearchTree.h>
#include <ZMasher\CollisionDefines.h>

struct __declspec(align(16)) SphereCollisionComponent
{
	float m_Radius;
	eCOLLISIONTYPE m_CollisionFilter;
	GameObject m_GameObject;
	CollisionCallBack m_CollisionCallback = nullptr;
};


class SphereCollisionComponentManager :
	public ComponentManager<SphereCollisionComponent>
{
public:
	SphereCollisionComponentManager();
	~SphereCollisionComponentManager();

	SphereCollisionComponent* AddComponent(	const eCOLLISIONTYPE filter,
						const float radius, 
						GameObject game_object,
						CollisionCallBack callback);
	void DeleteComponentCallback(GameObject component)override;
	bool Init()override;
	void Destroy()override;
	bool Update()override;

private:

	friend class CollisionSystem;
};