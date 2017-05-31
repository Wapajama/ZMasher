#pragma once
#include <ZMasher\BaseSystem.h>
#include <Math\ZMVector3.h>
#include <Math\ZMMatrix44.h>

class AISystem
	:public BaseSystem
{
public:
	AISystem(class AIComponentManager* ai,
			 class CollisionComponentManager* collision,
			 class TransformComponentManager* transform);
	~AISystem();

	bool Simulate(const float dt);

private:

	void FaceDirection(ZMasher::Matrix44f& transform, const ZMasher::Vector3f& direction);

	struct SteeringArgs
	{
		struct AIComponent* m_AIComp;
		const class AIType* m_Type;
		ZMasher::Vector3f m_ToTarget;
	};

	ZMasher::Vector3f Seek(const SteeringArgs& args);
	ZMasher::Vector3f Arrive(const SteeringArgs& args);

	void ClampMaxSpeed(ZMasher::Vector3f& steering, const class AIType* type);

	AIComponentManager* m_AIMngr;
	CollisionComponentManager* m_CollisionMngr;
	TransformComponentManager* m_TransformMngr;
	
};

