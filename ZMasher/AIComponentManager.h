#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMVector.h>
#include <DataStructures\BinarySearchTree.h>

enum eAIBehaviour
{
	ARRIVE,
	SEEK,
	FLEE,

	BEHAVIOUR_COUNT
};

enum eAIType
{
	ZOLDIER,

	AITYPE_COUNT
};

struct AIType
{
	float m_MaxSpeed;
	float m_ArrivingDist;//when the AI will start slowing down
	float m_ArrivedDist;//when the AI will stop
};

struct AIComponent
{
	GameObject m_GameObject;
	eAIType m_Type;//"Prefabs"
	ZMasher::Vector3f m_TargetPos;//could be used for search, arrive, flee, etc
	eAIBehaviour m_CurrentBehaviour;//TODO: switch to a decisiontree/statemachine pattern
};

struct AIIndexPair
{
	GameObject game_object;
	int index;
};

class AIComparer
	: public ZMasher::BSTComparator<AIIndexPair>
{
public:
	bool LessThan(const AIIndexPair& one,const AIIndexPair& two)const override
	{
		return one.game_object.m_ID < two.game_object.m_ID;
	}
	bool GreaterThan(const AIIndexPair& one,const AIIndexPair& two)const override
	{
		return one.game_object.m_ID > two.game_object.m_ID;
	}
	bool Equals(const AIIndexPair& one,const AIIndexPair& two)const override
	{
		return one.game_object == two.game_object;
	}
};

class AIComponentManager :
	public ComponentManager
{
public:
	AIComponentManager();
	~AIComponentManager();

	void AddComponent(GameObject game_object, eAIType type, 
					  const ZMasher::Vector3f& target_pos = ZMasher::Vector3f(0,0,0));

	AIComponent* GetComponent(GameObject game_object);
	const AIType* GetAIType(eAIType type);
	bool Init()override;
	bool Update()override;
	void RemoveComponentWithGameObject(GameObject object, bool directly = false)override;

private:

	void HardcodedAITypes();

	friend class AISystem;
	GrowArray<AIType> m_AITypes;
	GrowArray<AIComponent, short, 512> m_AIComponents;
	ZMasher::BinarySearchTree<AIIndexPair, AIComparer> m_LookupSet;
};

