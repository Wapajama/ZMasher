#pragma once
#include <stdint.h>
typedef unsigned __int64 GO_ID_TYPE;
typedef unsigned short GO_GEN_TYPE;
const GO_ID_TYPE GAMEOBJECT_ID_BITS = 48i64;
const GO_ID_TYPE GAMEOBJECT_ID_MASK = (1i64 << GAMEOBJECT_ID_BITS) - 1i64;

const GO_ID_TYPE GAMEOBJECT_GEN_BITS = 16i64;
const GO_ID_TYPE GAMEOBJECT_GEN_MASK = (1i64 << GAMEOBJECT_GEN_BITS) - 1i64;
#define MAX_GAME_OBJS (1024*64) - 1
#define MAX_COMPONENTS (1024*64) - 1
#define GAME_OBJS_INDEX __int32 // For referencing game objects in a list
#define GO_ARRAY_PARAMS int, MAX_GAME_OBJS
#define COMP_ARRAY_PARAMS int, MAX_COMPONENTS

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

	inline bool operator==(const GameObject& game_object)const
	{
		return this->m_ID == game_object.m_ID;
	}
	inline bool operator!=(const GameObject& game_object)const
	{
		return (!((*this) == game_object));
	}

	inline GO_ID_TYPE Index() const
	{
		return m_ID & GAMEOBJECT_ID_MASK;
	}
	inline GO_GEN_TYPE Generation() const
	{
		return (m_ID >> GAMEOBJECT_ID_BITS) & GAMEOBJECT_GEN_MASK;
	}
};

constexpr GameObject g_NullGameObject = { INT64_MAX };
#define NULL_GAME_OBJECT g_NullGameObject