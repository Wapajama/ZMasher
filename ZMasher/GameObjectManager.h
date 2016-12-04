#pragma once
#include "TransformComponentManager.h"

//Dataoriented entitysystem

typedef unsigned int ID_TYPE;
#define NUMBER_OF_GAME_OBJECTS 1024
struct GameObject
{
	ID_TYPE m_ID;
};

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Update(const float dt);

	bool Init();
	void Destroy();

	GameObject CreateGameObject();

	bool IsAlive(GameObject game_object);
	void Destroy(GameObject game_Object);

private:
	ID_TYPE m_CurrentID;

	TransformComponentManager m_TransformMngr;
	GameObject m_GameObjects[NUMBER_OF_GAME_OBJECTS];
};

/*
	command.Add(eCommands::SetPosition, data);
	command.Add(eCommands::

*/