#pragma once
#include <ZMUtils/Math/ZMVector3.h>
#include <ZMUtils/Math/ZMVector4.h>
#include <GameObject.h>
struct CollCallbackArgs
{
	class SphereCollisionComponent* a;
	SphereCollisionComponent* b;
	class MomentumComponent* x;
	MomentumComponent* y;
};

typedef void(*CollisionCallBack)(CollCallbackArgs args);

typedef unsigned short CollisionType;
enum eCOLLISIONTYPE: CollisionType
{
	eSphere = 1 << 0,
	eAABB = 1 << 1,
	eBlocker = 1 << 2
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