#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMMatrix44.h>
#include <ZMasher\GameObject.h>
#include <Time/Profiler.h>
//#include <DataStructures\BinarySearchTree.h>

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

struct TransformIndexPair
{
	GameObject game_object;
	int index;//index in the array in TransformComponentManager
};

class TransformComparer
	: public ZMasher::BSTComparator<TransformIndexPair>
{
public:
	bool LessThan(const TransformIndexPair& one,const TransformIndexPair& two)const override
	{
		return one.game_object.m_ID < two.game_object.m_ID;
	}
	bool GreaterThan(const TransformIndexPair& one,const TransformIndexPair& two)const override
	{
		return one.game_object.m_ID > two.game_object.m_ID;
	}
	bool Equals(const TransformIndexPair& one,const TransformIndexPair& two)const override
	{
		return one.game_object == two.game_object;
	}

};

class TransformComponentManager :
	public ComponentManager<
		TransformComponent>
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
	//TransformComponent* GetTransformComp(GameObject game_object);

protected:
	//void RemoveComponentWithGameObjectInternal(GameObject object)override;

private:

	//ZMasher::BinarySearchTree<TransformIndexPair, TransformComparer> m_LookupSet;
	//GrowArray<TransformComponent, short, 1024> m_Transforms;
	ProfilerTaskID m_GetTransformTimeStamp;
};
