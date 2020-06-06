#pragma once
//const int GAME_OBJECT_ALIVE_BIT = (1 << 31);
//#define GAME_OBJECT_IS_ALIVE(game_object_id) static_cast<bool>(game_object_id.m_ID & GAME_OBJECT_ALIVE_BIT)
//#define GAME_OBJECT_TOGGLE_ALIVE_GO(go) go.m_ID = go.m_ID ^ GAME_OBJECT_ALIVE_BIT
//#define GAME_OBJECT_KILL(go) GAME_OBJECT_IS_ALIVE(go)? GAME_OBJECT_TOGGLE_ALIVE_GO(go) : 0
//#define GAME_OBJECT_SET_ALIVE(go) GAME_OBJECT_IS_ALIVE(go)? 0: GAME_OBJECT_TOGGLE_ALIVE_GO(go)
#include <stdint.h>
typedef unsigned __int64 GO_ID_TYPE;
typedef unsigned short GO_GEN_TYPE;
const GO_ID_TYPE GAMEOBJECT_ID_BITS = 48;
const GO_ID_TYPE GAMEOBJECT_ID_MASK = (1 << GAMEOBJECT_ID_BITS) - 1;

const GO_ID_TYPE GAMEOBJECT_GEN_BITS = 16;
const GO_ID_TYPE GAMEOBJECT_GEN_MASK = (1 << GAMEOBJECT_GEN_BITS) - 1;

//#define NUMBER_OF_GAME_OBJECTS 1024

struct GameObject
{
	union
	{
	public:
		// 48 bits + 16 bits
		// 48 first are ID
		// 16 last are Generation, makes room for 65536 generations
		GO_ID_TYPE m_ID;
		struct
		{
			// these three shorts are only made to "push up" the generation short
			unsigned short m_IDShort0;
			unsigned short m_IDShort1;
			unsigned short m_IDShort2;
			unsigned short m_Gen;
		};
	};

	bool operator==(const GameObject& game_object)const
	{
		////make sure they are both dead first
		//GameObject first = *this;
		//if (GAME_OBJECT_IS_ALIVE(first))
		//{
		//	GAME_OBJECT_TOGGLE_ALIVE_GO(first);
		//}
		//GameObject second = game_object;
		//if (GAME_OBJECT_IS_ALIVE(second))
		//{
		//	GAME_OBJECT_TOGGLE_ALIVE_GO(second);
		//}
		return this->m_ID == game_object.m_ID;
	}
	bool operator!=(const GameObject game_object)const
	{
		return (!((*this) == game_object));
	}

	GO_ID_TYPE Index() const
	{
		return m_ID & GAMEOBJECT_ID_MASK;
	}
	GO_GEN_TYPE Generation() const
	{
		return (m_ID >> GAMEOBJECT_ID_BITS) & GAMEOBJECT_GEN_MASK;
	}
};

constexpr GameObject g_NullGameObject = { INT64_MAX };
#define NULL_GAME_OBJECT g_NullGameObject