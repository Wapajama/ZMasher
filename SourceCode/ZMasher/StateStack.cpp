#include "StateStack.h"
#include <GameState.h>


StateStack::StateStack()
{
}


StateStack::~StateStack()
{
}

bool StateStack::Update(float dt)
{
	for (int i = 0; i < m_GameStates.Size(); i++)
	{
		if (!m_GameStates[i]->Update(dt))
		{

			return false;
		}
	}
}

void StateStack::PushState(GameState* gameState)
{
	m_GameStates.Add(gameState);
}
void StateStack::PopState(GameState* gameState)
{
	for (int i = 0; i < m_GameStates.Size(); i++)
	{
		if (m_GameStates[i] == gameState)
		{
			m_GameStates[i]->Destroy();
			m_GameStates.RemoveCyclic(i);
			break;
		}
	}
}

void StateStack::Exit()
{
	for (int i = 0; i < m_GameStates.Size(); i++)
	{
		m_GameStates[i]->Destroy();
	}
	m_GameStates.RemoveAll();
}