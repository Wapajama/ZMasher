#pragma once
#include "TransformComponentManager.h"
#include "MeshComponentManager.h"
//Dataoriented entitysystem

const int GAME_OBJECT_ALIVE_BIT = (1 << 31);
#define GM_TOGGLE_ALIVE_GO(go) go = go ^ GAME_OBJECT_ALIVE_BIT
typedef unsigned int ID_TYPE;
#define NUMBER_OF_GAME_OBJECTS 1024
struct GameObject
{
	ID_TYPE m_ID;
};

enum eComponentType : int
{
	eTransformComponent = 1 << 1,
	eMeshComponent = 1 << 2
};

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Update(const float dt);

	bool Init();
	void Destroy();

	GameObject CreateGameObject(const int components);

	bool IsAlive(GameObject game_object);
	void Destroy(GameObject& game_Object);

private:
	ID_TYPE m_CurrentID;

	TransformComponentManager m_TransformManager;
	MeshComponentManager m_MeshManager;
	GameObject m_GameObjects[NUMBER_OF_GAME_OBJECTS];

};

/*
	command.Add(eCommands::SetPosition, data);
	command.Add(eCommands::

*/