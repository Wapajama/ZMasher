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

bool AIComponentManager::Init()
{
	return true;
}

bool AIComponentManager::Update()
{
	RemoveDeadComponents(m_AIComponents);
	return true;
}
