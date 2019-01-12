#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <ZMasher\GameObject.h>

struct BulletComponent//probably should be divided into other components, lets just see how this pans out
{
	BulletComponent() {}
	BulletComponent(const GameObject game_object, const float speed, const float damage, const float life_time)
	{
		m_GameObject = game_object;
		m_Speed = speed;
		m_Damage = damage;
		m_LifeTime = life_time;
	}

	GameObject m_GameObject;
	float m_Speed;
	float m_Damage;
	float m_LifeTime;
};


class BulletComponentManager :
	public ComponentManager<
		BulletComponent>
		
{
	friend class BulletSystem;
public:
	BulletComponentManager();
	~BulletComponentManager();

	bool AddComponent(const GameObject game_object, const float speed, const float damage, const float life_time = 10.f);

	bool Init()override;
	void Destroy()override;
	bool Update()override;
	
private:
	//void RemoveComponentWithGameObjectInternal(GameObject object)override;
	//GrowArray<BulletComponent> m_Bullets;
};

