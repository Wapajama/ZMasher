#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMVector.h>


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

class AIComponentManager :
	public ComponentManager
{
public:
	AIComponentManager();
	~AIComponentManager();

	void AddComponent(GameObject game_object, eAIType type, 
					  const ZMasher::Vector3f& target_pos = ZMasher::Vector3f(0,0,0));

	//TODO: optimize this, currently bruteforced
	AIComponent* GetComponent(GameObject game_object);
	const AIType* GetAIType(eAIType type);
	bool Init();
	bool Update();

private:

	void HardcodedAITypes();

	friend class AISystem;
	GrowArray<AIType> m_AITypes;
	GrowArray<AIComponent> m_AIComponents;
};

