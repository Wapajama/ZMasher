#pragma once
#include <ZMasher\BaseSystem.h>
#include <Math\ZMVector3.h>
#include <Math\ZMMatrix44.h>
#include <Time/Timer.h>
#include <ZMasher/GameObject.h>
#include <ZMasher/AIGroup.h>
#include <GlobalIncludes/project_defines.h>


class AISystem
	:public BaseSystem
{
public:
	AISystem(	class AIComponentManager* ai,
				class SphereCollisionComponentManager* sphere_collision,
				class MomentumComponentManager* momentum,
				class TransformComponentManager* transform,
				class CollisionSystem* coll_system);
	~AISystem();

	bool Init(void* arguments)override;
	bool Destroy()override;
	bool Simulate(const float dt);

	AIGroup* CreateAIs(AIObjectArgs* args, int count);
private:
	struct AIBehaviourArgs
	{
		AIObject* obj;
		const float dt;
	};

	struct TurretArgs
	{
		GameObject target;
		struct CollisionQuery* query;
	};

	void HitlerBehaviour(const AIBehaviourArgs& args);
	void BasicTurretBehaviour(const AIBehaviourArgs& args);

#ifdef BENCHMARK
	Timer m_AIInternalTimeStamp;
#endif // BENCHMARK


	void FaceDirection(ZMasher::Matrix44f& transform, const ZMasher::Vector3f& direction, const float dt);

	struct SteeringArgs
	{
		struct AIComponent* m_AIComp;
		const class AIType* m_Type;
		ZMasher::Vector3f m_ToTarget;
	};

	void AddNewZoldier();

	ZMasher::Vector3f Seek(const SteeringArgs& args);
	ZMasher::Vector3f Arrive(const SteeringArgs& args);
	void SpawnBullet(const AIBehaviourArgs & args);

	void ClampMaxSpeed(ZMasher::Vector3f& steering, const class AIType* type);
	bool IsInAnyOfAIGroups(int i);
	class AIComponentManager* m_AIMngr;
	class SphereCollisionComponentManager* m_SphereCollisionMngr;
	class MomentumComponentManager* m_MomentumMngr;
	class TransformComponentManager* m_TransformMngr;
	class CollisionSystem* m_CollSystem;
	GrowArray<AIGroup> m_AIGroups;

};