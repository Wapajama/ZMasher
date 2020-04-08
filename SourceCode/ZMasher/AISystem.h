#pragma once
#include <ZMasher\BaseSystem.h>
#include <Math\ZMVector3.h>
#include <Math\ZMMatrix44.h>
#include <Time/Profiler.h>
#include <ZMasher/GameObject.h>
class AISystem
	:public BaseSystem
{
public:
	AISystem(	class AIComponentManager* ai,
				class SphereCollisionComponentManager* sphere_collision,
				class MomentumComponentManager* momentum,
				class TransformComponentManager* transform);
	~AISystem();

	bool Init(void* arguments)override;
	bool Simulate(const float dt);

private:
	struct AIBehaviourArgs
	{
		class TransformComponent* transform_comp;
		class AIComponent* ai_comp;
		GameObject game_object;
	};

	void HitlerBehaviour(const AIBehaviourArgs& args);
	void BasicTurretBehaviour(const AIBehaviourArgs& args);

	ProfilerTaskID m_AIInternalTimeStamp;

	void FaceDirection(ZMasher::Matrix44f& transform, const ZMasher::Vector3f& direction);

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

	class AIComponentManager* m_AIMngr;
	class SphereCollisionComponentManager* m_SphereCollisionMngr;
	class MomentumComponentManager* m_MomentumMngr;
	class TransformComponentManager* m_TransformMngr;
	
};

