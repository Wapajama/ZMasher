#pragma once
#include "GameState.h"


class ModelViewerState :
	public GameState
{
public:
	ModelViewerState();
	~ModelViewerState();

	bool Init(const char* args)override;
	bool Update(const float dt)override;
	bool Destroy()override { return true; }
};

