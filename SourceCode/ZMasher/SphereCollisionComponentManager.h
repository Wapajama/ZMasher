#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMVector3.h>
#include <DataStructures\BinarySearchTree.h>
#include <ZMasher\CollisionDefines.h>

struct SphereCollisionComponent
{
	//DONT CHANGE THE ORDER OF THE VARIABLES
	CollisionType m_CollisionFilter;
	float m_Radius;
	GameObject m_GameObject;
	CollisionCallBack m_CollisionCallback = nullptr;
	//index for the collision info array in the collision system. -1 if nothing, else > 0
	int collInfoIndex = -1;
};


class SphereCollisionComponentManager :
	public ComponentManager<SphereCollisionComponent>
{
public:
	SphereCollisionComponentManager();
	~SphereCollisionComponentManager();

	SphereCollisionComponent* AddComponent(	const CollisionType filter,
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

