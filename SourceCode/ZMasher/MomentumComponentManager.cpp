#include "MomentumComponentManager.h"



MomentumComponentManager::MomentumComponentManager()
{
}


MomentumComponentManager::~MomentumComponentManager()
{
}

MomentumComponent* MomentumComponentManager::AddComponent(
	GameObject game_object,
	const float weight,
	const ZMasher::Vector3f& speed)
{
	ComponentManager::AddComponent({ game_object , weight, speed, ZMasher::Vector3f() }, game_object);
	return &m_Components.GetLast();
}