#pragma once

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