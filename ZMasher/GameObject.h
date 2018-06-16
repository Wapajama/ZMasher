#pragma once
const int GAME_OBJECT_ALIVE_BIT = (1 << 31);
#define GAME_OBJECT_IS_ALIVE(game_object_id) static_cast<bool>(game_object_id.m_ID & GAME_OBJECT_ALIVE_BIT)
#define GAME_OBJECT_TOGGLE_ALIVE_GO(go) go.m_ID = go.m_ID ^ GAME_OBJECT_ALIVE_BIT
typedef unsigned int ID_TYPE;
#define NUMBER_OF_GAME_OBJECTS 1024

struct GameObject
{
	ID_TYPE m_ID;

	bool operator==(const GameObject& game_object)const
	{
		//make sure they are both dead first
		GameObject first = *this;
		if (GAME_OBJECT_IS_ALIVE(first))
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(first);
		}
		GameObject second = game_object;
		if (GAME_OBJECT_IS_ALIVE(second))
		{
			GAME_OBJECT_TOGGLE_ALIVE_GO(second);
		}
		return first.m_ID == second.m_ID;
	}
	bool operator!=(const GameObject game_object)const
	{
		return (!((*this) == game_object));
	}

};

constexpr GameObject g_NullGameObject = { 0 };
#define NULL_GAME_OBJECT g_NullGameObject