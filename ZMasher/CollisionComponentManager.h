#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMVector3.h>
#include <DataStructures\BinarySearchTree.h>

typedef unsigned short CollisionType;
enum eCOLLISIONTYPE: CollisionType
{
	eSphere = 1 << 0,
	eAABB = 1 << 1,
};

struct SphereCollisionComponent
{
	SphereCollisionComponent(const CollisionType filter, 
							 const float radius, 
							 GameObject game_object):
		 m_CollisionFilter(filter)
		, m_Radius(radius)
		, m_GameObject(game_object)
	{}
	SphereCollisionComponent() {}
	CollisionType m_CollisionFilter;
	float m_Radius;
	GameObject m_GameObject;
};

struct MomentumComponent //no need for gameobject id, they are aligned in the same order
{
	MomentumComponent(GameObject game_object,
					  const float weight,
					  const ZMasher::Vector3f& speed) 
		: m_GameObject(game_object)
		, m_Weight(weight)
	{
		m_Speed = speed;
	}
	MomentumComponent() {}
	GameObject m_GameObject;
	float m_Weight;
	ZMasher::Vector3f m_Speed;
	ZMasher::Vector3f m_Acceleration;
};

struct SphereIndexPair
{
	GameObject game_object;
	int index;
};

class SphereComparer
	: public ZMasher::BSTComparator<SphereIndexPair>
{
public:
	bool LessThan(const SphereIndexPair& one,const SphereIndexPair& two)const override
	{
		return one.game_object.m_ID < two.game_object.m_ID;
	}
	bool GreaterThan(const SphereIndexPair& one,const SphereIndexPair& two)const override
	{
		return one.game_object.m_ID > two.game_object.m_ID;
	}
	bool Equals(const SphereIndexPair& one,const SphereIndexPair& two)const override
	{
		return one.game_object == two.game_object;
	}
};

struct CollisionInfoComponent
{
	CollisionInfoComponent(GameObject object, GameObject collide_object) :
		m_Object(object),
		m_CollideObject(collide_object){}
	CollisionInfoComponent() {}
	GameObject m_Object;
	GameObject m_CollideObject;
};

class CollisionComponentManager :
	public ComponentManager
{
public:
	CollisionComponentManager();
	~CollisionComponentManager();

	void AddComponent(const CollisionType filter, 
					  const float radius, 
					  GameObject game_object,
					  const float weight,
					  const ZMasher::Vector3f& speed = ZMasher::Vector3f());

	bool Init()override;
	void Destroy()override;
	bool Update()override;

	MomentumComponent* GetMomentumComponent(GameObject game_object);
	SphereCollisionComponent* GetSphereCollisionComponent(GameObject game_object);

private:

	GrowArray<SphereCollisionComponent, short, 512> m_Spheres;
	GrowArray<MomentumComponent, short, 512> m_Momentums;
	GrowArray<CollisionInfoComponent, short, 512> m_Collisions;

	ZMasher::BinarySearchTree<SphereIndexPair, SphereComparer> m_LookupSet;

	friend class CollisionSystem;
};

