#pragma once
#include <ZMasher\dllHeader.h>

class ZMASHER_DLL GameState
{
public:
	GameState();
	~GameState();

	virtual bool Init(const char* args) = 0;
	virtual bool Update(const float dt) = 0;
	virtual bool Destroy() { return true; }
};

