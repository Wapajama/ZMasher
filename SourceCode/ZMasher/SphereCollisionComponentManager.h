#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMVector3.h>
#include <DataStructures\BinarySearchTree.h>

struct CollCallbackArgs
{
	class SphereCollisionComponent* a;
	SphereCollisionComponent* b;
	class MomentumComponent* x;
	MomentumComponent* y;
};

typedef void(*CollisionCallBack)(CollCallbackArgs args);

typedef unsigned short CollisionType;
enum eCOLLISIONTYPE: CollisionType
{
	eSphere = 1 << 0,
	eAABB = 1 << 1,
};

struct SphereCollisionComponent
{
	CollisionType m_CollisionFilter;
	float m_Radius;
	GameObject m_GameObject;
	CollisionCallBack m_CollisionCallback = nullptr;
};

//struct SphereIndexPair
//{
//	GameObject game_object;
//	int index;
//};
//
//class SphereComparer
//	: public ZMasher::BSTComparator<SphereIndexPair>
//{
//public:
//	bool LessThan(const SphereIndexPair& one,const SphereIndexPair& two)const override
//	{
//		return one.game_object.m_ID < two.game_object.m_ID;
//	}
//	bool GreaterThan(const SphereIndexPair& one,const SphereIndexPair& two)const override
//	{
//		return one.game_object.m_ID > two.game_object.m_ID;
//	}
//	bool Equals(const SphereIndexPair& one,const SphereIndexPair& two)const override
//	{
//		return one.game_object == two.game_object;
//	}
//};

class SphereCollisionComponentManager :
	public ComponentManager<SphereCollisionComponent>
{
public:
	SphereCollisionComponentManager();
	~SphereCollisionComponentManager();

	void AddComponent(	const CollisionType filter, 
						const float radius, 
						GameObject game_object,
						CollisionCallBack callback);

	bool Init()override;
	void Destroy()override;
	bool Update()override;
	//void RemoveComponentWithGameObject(GameObject object, bool directly = false)override;
	//MomentumComponent* GetMomentumComponent(GameObject game_object);
	//SphereCollisionComponent* GetSphereCollisionComponent(GameObject game_object);

private:
	//void RemoveComponentWithGameObjectInternal(GameObject object)override;

	//GrowArray<SphereCollisionComponent, short, 512> m_Spheres;
	//GrowArray<MomentumComponent, short, 512> m_Momentums;

	//ZMasher::BinarySearchTree<SphereIndexPair, SphereComparer> m_LookupSet;

	friend class CollisionSystem;
};

