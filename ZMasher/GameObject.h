#pragma once
const int GAME_OBJECT_ALIVE_BIT = (1 << 31);
#define GM_TOGGLE_ALIVE_GO(go) go = go ^ GAME_OBJECT_ALIVE_BIT
typedef unsigned int ID_TYPE;
#define NUMBER_OF_GAME_OBJECTS 1024

struct GameObject
{
	ID_TYPE m_ID;

	bool operator==(const GameObject& game_object)
	{
		return m_ID == game_object.m_ID;
	}

};