#pragma once

class BaseSystem
{
public:
	BaseSystem();
	~BaseSystem();

	virtual bool Init(void* arguments){return true;};
	virtual bool Simulate(const float dt)=0{return false;};
	
private:

};

