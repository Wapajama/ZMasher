#include "BaseSystem.h"



BaseSystem::BaseSystem()
{
}


BaseSystem::~BaseSystem()
{
}

GrowArray<ComponentCollection*> BaseSystem::m_ComponentCollections(1024);