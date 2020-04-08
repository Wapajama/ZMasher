#include "AIComponentManager.h"
#include <ZMasher\GameObjectManager.h>

AIComponentManager::AIComponentManager() :
	ComponentManager(1024)
{
}

AIComponentManager::~AIComponentManager()
{
}

void AIComponentManager::AddComponent(GameObject game_object, eAIType type, 
					const ZMasher::Vector3f& target_pos)
{
	ComponentManager::AddComponent({ game_object, type, target_pos }, game_object);
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
	m_AITypes[eAIType::ZOLDIER].m_ArrivedDist = 15.0f;
	m_AITypes[eAIType::ZOLDIER].m_ArrivingDist = 30.f;
	m_AITypes[eAIType::ZOLDIER].m_MaxSpeed = 10.f;
}

bool AIComponentManager::Update()
{
	return ComponentManager::Update();
}

short AIComponentManager::GetNumberOfAIs()
{
	short n_ais = 0;
	for (short i = 0; i < m_Components.Size(); i++)
	{
		if (GameObjectManager::Instance()->Alive(m_Components[i].m_GameObject))
		{
			++n_ais;
		}
	}
	return n_ais;
}
