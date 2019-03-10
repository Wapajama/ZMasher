#pragma once
#include <ZMUtils/DataStructures/GrowArray.h>

class GameState;

class StateStack
{
public:
	StateStack();
	~StateStack();

	bool Update(float dt);

	void PushState(GameState* gameState);
	void PopState(GameState* gameState);

	void Exit();

private:
	GrowArray<GameState*> m_GameStates;
};

