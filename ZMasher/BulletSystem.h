#pragma once

class BulletComponentManager;
class TransformComponentManager;
class GameObjectManager;

class BulletSystem//Pretty stupid name and responsibility, 
{
public:
	BulletSystem(BulletComponentManager* bullet_comp_manager, TransformComponentManager* transform_comp_manager);
	~BulletSystem();

	bool Simulate(const float dt, GameObjectManager* go_man);

private:

	BulletComponentManager* m_BulletCompManager;
	TransformComponentManager* m_TransformCompManager;
};

