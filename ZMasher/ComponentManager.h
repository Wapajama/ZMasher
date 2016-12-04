#pragma once
class ComponentManager
{
public:
	ComponentManager();
	virtual ~ComponentManager();

	virtual bool Init()=0;
	virtual void Destroy(){};

};

