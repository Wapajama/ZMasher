#pragma once
class ComponentManager
{
public:
	ComponentManager();
	virtual ~ComponentManager();

	virtual bool Init()=0;
	virtual void Destroy(){};
	virtual bool Update() {return true;}//TODO: MAKE PURE VIRTUAL

};

