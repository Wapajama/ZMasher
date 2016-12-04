#pragma once
#include "ComponentManager.h"

class TransformComponentManager :
	public ComponentManager
{
public:
	TransformComponentManager();
	~TransformComponentManager();

	bool Init()override;
	void Destroy()override;
};

