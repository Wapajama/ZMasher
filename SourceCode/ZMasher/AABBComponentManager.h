#pragma once
#include "ComponentManager.h"
#include <ZMasher\CollisionDefines.h>

struct AABBCollisionComponent
{
	// Translation in transformcomponent 
	float r[3]; // radius/halfwidth for x,y,z
	CollisionType m_CollisionFilter;
	GameObject m_GameObject;
	CollisionCallBack m_CollisionCallback = nullptr;

	int collInfoIndex; // TODO, Keep working on this system
};

class AABBComponentManager :
	public ComponentManager<AABBCollisionComponent>
{
public:
	AABBComponentManager();
	~AABBComponentManager();

	void AddComponent(const CollisionType filter,
		const float halfwidth[3], 
		GameObject game_object,
		CollisionCallBack callback);
	bool Init()override;
	void Destroy()override;
	bool Update()override;

private:

	friend class CollisionSystem;

};

