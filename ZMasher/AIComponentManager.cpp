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
	m_AIComponents.Add({game_object, type, target_pos});
}

AIComponent* AIComponentManager::GetComponent(GameObject game_object)
{
	for (short i = 0; i < m_AIComponents.Size(); ++i)
	{
		if (m_AIComponents[i].m_GameObject == game_object)
		{
			return &m_AIComponents[i];
		}
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
