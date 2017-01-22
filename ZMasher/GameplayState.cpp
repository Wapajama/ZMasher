#include "GameplayState.h"
#include <ZMasher\ZMModelFactory.h>

GameplayState::GameplayState()
{
}


GameplayState::~GameplayState()
{
}

bool GameplayState::Init(const char* args)
{
	ZMasher::Vector4f position(0, 1, 0.f, 1.f);
	ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	for (int i = 0; i < 10; ++i)
	{
		///m_ModelInstances[m_ModelInstances.size() - 1]->SetPosition(position + ZMasher::Vector3f(-500 + i * 100, 0, 0));
		GameObject new_object = m_GameObjectManager.CreateGameObject();
		m_GameObjects.Add(new_object);
		transform.SetTranslation(position + ZMasher::Vector4f(-500 + i * 100, 0, 0,1.f));
		m_GameObjectManager.TransformManager()->AddComponent(new_object, transform);
		m_GameObjectManager.MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance("../data/Truax_Studio_Mac11.FBX"));
	}

	return true;
}

bool GameplayState::Update(const float dt)
{
	m_GameObjectManager.Update(dt);
	return true;
}