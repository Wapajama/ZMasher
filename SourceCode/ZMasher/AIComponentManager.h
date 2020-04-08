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
	ATTACK,

	BEHAVIOUR_COUNT
};

enum eAIType
{
	ZOLDIER,
	BASIC_TURRET,

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

class AIComponentManager :
	public ComponentManager<AIComponent>
		
{
public:
	AIComponentManager();
	~AIComponentManager();

	void AddComponent(GameObject game_object, eAIType type, 
					  const ZMasher::Vector3f& target_pos = ZMasher::Vector3f(0,0,0));

	const AIType* GetAIType(eAIType type);
	bool Init()override;
	bool Update()override;

	short GetNumberOfAIs();

private:
	void HardcodedAITypes();

	friend class AISystem;
	GrowArray<AIType> m_AITypes;
};

