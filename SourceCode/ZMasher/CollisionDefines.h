#pragma once
#include <ZMUtils/Math/ZMVector3.h>
#include <ZMUtils/Math/ZMVector4.h>
#include <GameObject.h>
struct CollCallbackArgs
{
	const class SphereCollisionComponent* __restrict a;
	const SphereCollisionComponent* __restrict b;
	const class MomentumComponent* __restrict x;
	const MomentumComponent* __restrict y;
};

typedef void(*CollisionCallBack)(CollCallbackArgs args);

typedef unsigned short CollisionType;
enum eCOLLISIONTYPE: CollisionType
{
	eSphere = 1 << 0,
	eAABB = 1 << 1,
	eBlocker = 1 << 2,
	eTurretBullet = 1 << 3,
	eTurret = 1 << 4,
	eEnemy = 1 << 5,
};

enum eQueryType
{
	eClosest = 1 << 0
};

struct CollisionInfoStruct
{
	GameObject objectA;
	GameObject objectB;
	ZMasher::Vector3f posA;
	ZMasher::Vector3f posB;
};

enum CollisionQueryType
{
	eSPHERE
};

struct CollisionQueryArgs
{
	CollisionQueryType type;
};

struct SphereQueryArgs : public CollisionQueryArgs
{
	ZMasher::Vector4f position;
	float r;
};

struct CollisionQuery
{
	GameObject owner;
	GameObject target;
	CollisionQueryArgs* args;
};