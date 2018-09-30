#include "AIComponentManager.h"



AIComponentManager::AIComponentManager()
{
}


AIComponentManager::~AIComponentManager()
{
}

void AIComponentManager::AddComponent(GameObject game_object, eAIType type, 
					const ZMasher::Vector3f& target_pos)
{
	const int free_index = PopFreeIndex();

	if (free_index == m_FreeIndexes.found_none)
	{
		m_AIComponents.Add({ game_object, type, target_pos });
	}
	else
	{
		m_AIComponents[free_index] = { game_object, type, target_pos };
	}
	m_LookupSet.Insert({game_object, m_AIComponents.Size()-1});
}

AIComponent* AIComponentManager::GetComponent(GameObject game_object)
{
	ZMasher::BSTNode<AIIndexPair,AIComparer>* ai = m_LookupSet.Find({ game_object, -1 });
	if (ai)
	{
		return &m_AIComponents[ai->value.index];
	}
	return nullptr;
}

const AIType* AIComponentManager::GetAIType(eAIType type)
{
	return &m_AITypes[type];
}

bool AIComponentManager::Init()
{
	for (short i = 0; i < eAIType::AITYPE_COUNT; i++)
	{
		m_AITypes.Add(AIType());
	}

	HardcodedAITypes();

	return true;
}

void AIComponentManager::HardcodedAITypes()
{
	m_AITypes[eAIType::ZOLDIER].m_ArrivedDist = 3.0f;
	m_AITypes[eAIType::ZOLDIER].m_ArrivingDist = 30.f;
	m_AITypes[eAIType::ZOLDIER].m_MaxSpeed = 30.f;
}

bool AIComponentManager::Update()
{
	RemoveDeadComponents(m_AIComponents);
	return true;
}

short AIComponentManager::GetNumberOfAIs()
{
	short n_ais = 0;
	for (short i = 0; i < m_AIComponents.Size(); i++)
	{
		if (GAME_OBJECT_IS_ALIVE(m_AIComponents[i].m_GameObject))
		{
			++n_ais;
		}
	}

	return n_ais;
}

void AIComponentManager::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	if (directly)
	{
		for (int i = 0; i < m_AIComponents.Size(); i++)
		{
			if (m_AIComponents[i].m_GameObject == object)
			{
				//m_AIComponents.RemoveCyclic(i);
				GAME_OBJECT_KILL(m_AIComponents[i].m_GameObject);
				break;
			}
		}
		return;
	}
	AIComponent* ai = GetComponent(object);
	if (ai)
	{
		GAME_OBJECT_KILL(ai->m_GameObject);
	}
}