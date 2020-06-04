#pragma once
#include <ZMUtils/DataStructures/GrowArray.h>

class ComponentCollection;

class BaseSystem
{
public:
	BaseSystem();
	~BaseSystem();

	virtual bool Init(void* arguments){return true;};
	virtual bool Simulate(const float dt)=0{return false;};
	virtual bool Destroy(){return true;}

protected:

	static GrowArray<ComponentCollection*> m_ComponentCollections;
};
