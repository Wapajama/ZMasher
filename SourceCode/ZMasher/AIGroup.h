#pragma once
#include "AllocationGroup.h"
#include <string>
#include <DataStructures/GrowArray.h>
#include <ZMUtils/Math/ZMMatrix44.h>
#include <CollisionDefines.h>
#include <GameObject.h>
struct AIObjectArgs
{
	ZMasher::Matrix44f spawnOrientation;
	ZMasher::Vector3f targetPos;

	// Mesh component
	std::string modelName;

	// Sphere component
	float radius;

	// Momentum component
	float speed;

	int aiType;

	int collisionType;
};

struct AIObject
{
	struct TransformComponent* tf;
	struct ModelComponent* mesh;
	struct AIComponent* ai;
	struct SphereCollisionComponent* sphere;
	struct MomentumComponent* momentum;
	GameObject go;
	class CollisionQuery* query;
};

class AIGroup :
	public AllocationGroup
{
public:
	AIGroup(const int size = 1024);
	~AIGroup();

	bool Iterate()override;
	void Allocate(const int size, void* args)override;
	inline AIObject* GetCurrentObject() { return &m_AIObjects[m_CurrentAIObject]; };

	__forceinline int GetAIFirst() {return aiFirst;}
	__forceinline int GetAILast() { return aiLast; }

private:

	GameObject	gFirst, gLast;
	int			meshFirst, meshLast;
	int			aiFirst, aiLast;
	int			sphereFirst, sphereLast;
	int			momFirst, momLast;

	int m_CurrentAIObject;
	GrowArray<AIObject> m_AIObjects;

};

