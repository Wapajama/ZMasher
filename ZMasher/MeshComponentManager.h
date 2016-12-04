#pragma once
#include "ComponentManager.h"


class MeshComponentManager :
	public ComponentManager
{
public:
	MeshComponentManager();
	~MeshComponentManager();

	bool Init()override;
	void Destroy()override;

private:
};

