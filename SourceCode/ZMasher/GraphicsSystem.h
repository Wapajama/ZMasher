#pragma once
#include "BaseSystem.h"
class GraphicsSystem :
	public BaseSystem
{
public:
	GraphicsSystem();
	~GraphicsSystem();

	bool Init(void* args)override;
	bool Simulate(const float dt)override;
	bool Destroy()override;

private:

};

