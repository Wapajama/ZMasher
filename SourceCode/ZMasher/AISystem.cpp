#include "AISystem.h"
#include <ZMasher/AIComponentManager.h>
#include <ZMasher/SphereCollisionComponentManager.h>
#include <ZMasher/MomentumComponentManager.h>
#include <ZMasher/TransformComponentManager.h>
#include <ZMasher/GameObjectManager.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>

extern void(*g_TestCallBack)(CollCallbackArgs);

AISystem::AISystem(AIComponentManager* ai,
				   SphereCollisionComponentManager* sphere_collision,
				   MomentumComponentManager* momentum,
				   TransformComponentManager* transform,
				   CollisionSystem* coll_system)
	: m_AIMngr(ai)
	, m_SphereCollisionMngr(sphere_collision)
	, m_MomentumMngr(momentum)
	, m_TransformMngr(transform)
	, m_CollSystem(coll_system)
	, m_AIGroups(1024)
{

}


AISystem::~AISystem()
{
}


bool AISystem::Init(void*)
{
	return true;
}

bool AISystem::Destroy()
{
#ifdef BENCHMARK
	Profiler::Instance()->AddTimeStamp(m_AIInternalTimeStamp, "AISystemInternal");
#endif // BENCHMARK

	return true;
}

const float ai_range = 300.f;

AIGroup* AISystem::CreateAIs(AIObjectArgs* args, int count)
{
	if (count == 0)
	{
		return nullptr;
	}
	AIGroup* ag = new AIGroup();
	ag->Allocate(count, args);
	return nullptr;
}

bool AISystem::IsInAnyOfAIGroups(int i)
{
	for (int i = 0; i < m_AIGroups.Size(); i++)
	{
		if (i >= m_AIGroups[i].GetAIFirst() &&
			i <= m_AIGroups[i].GetAILast())
		{
			return true;
		}
	}
	return false;
}

bool AISystem::Simulate(const float dt)
{
	START_TIME_STAMP(m_AIInternalTimeStamp);

	for (short i = 0; i < m_AIMngr->m_Components.Size(); i++)
	{
		const GameObject game_object = m_AIMngr->m_Components[i].m_GameObject;
		if (!GameObjectManager::Instance()->Alive(game_object))
		{
			continue;
		}
		TransformComponent* transform_comp = m_TransformMngr->GetComponent(game_object);
		if (transform_comp == nullptr)
		{
			continue;
		}
		AIComponent* ai_comp = m_AIMngr->GetComponent(game_object);
		if (!ai_comp)
		{
			continue;
		}
		switch (ai_comp->m_Type)
		{
		case eAIType::ZOLDIER:
		{
			AIObject obj = { transform_comp, nullptr, ai_comp, nullptr, m_MomentumMngr->GetComponent(game_object), game_object };
			HitlerBehaviour({ &obj,dt });
		}
		break;
		case eAIType::BASIC_TURRET:
		{
			AIObject obj = { transform_comp, nullptr, ai_comp, nullptr, m_MomentumMngr->GetComponent(game_object), game_object };
			BasicTurretBehaviour({ &obj, dt });
		}
			break;

		default:
			break;
		}
	}
	END_TIME_STAMP(m_AIInternalTimeStamp);
	return true;
}

void AISystem::HitlerBehaviour(const AIBehaviourArgs& a_args)
{
	AIObject* args = a_args.obj;
	const ZMasher::Vector3f position = ZMasher::Vector3f(args->tf->m_Transform.GetTranslation());
	const float length_to_target = (args->ai->m_TargetPos - position).Length();
	const AIType* ai_type = m_AIMngr->GetAIType(args->ai->m_Type);

	if (length_to_target < ai_type->m_ArrivedDist)
	{
		args->tf->m_Transform.SetTranslation(
			ZMasher::Vector4f(-ai_range, 
							  position.y, 
							  ZMasher::GetRandomFloat(-ai_range, ai_range), 
							  args->tf->m_Transform.GetTranslation().w));
		args->ai->m_TargetPos.z = ai_range * 0.5;
		return;
	}
	SteeringArgs steer_args{ args->ai, ai_type, args->ai->m_TargetPos - position };

	ZMasher::Vector3f steering(0, 0, 0);

	if (steer_args.m_ToTarget.Length() < steer_args.m_Type->m_ArrivingDist)
	{
		steering += Arrive(steer_args);
	} 
	else
	{
		steering += Seek(steer_args);
	}

	ClampMaxSpeed(steering, ai_type);
	args->momentum->m_Speed = steering;
	
	FaceDirection(args->tf->m_Transform, (position) - args->ai->m_LastFramePos, a_args.dt);
	args->ai->m_LastFramePos = position;
}

#ifndef _DEBUG
static float rof = 0.01f;
#else
static float rof = 0.1f;
#endif // !DEBUG

static float shootTimer = 0.f;

void AISystem::BasicTurretBehaviour(const AIBehaviourArgs & args)
{
	// Pseudo code
	CollisionQuery* q = m_CollSystem->GetQuery(args.obj->go);
	if (shootTimer > 0.f)
	{
		shootTimer -= args.dt;
	}
	if (q &&
		q->target != NULL_GAME_OBJECT &&
		q->target != q->owner &&
		GameObjectManager::Instance()->Alive(q->target))
	{
		TransformComponent* targetTransform = m_TransformMngr->GetComponent(q->target);

		if (targetTransform)
		{
			FaceDirection((args.obj->tf->m_Transform), targetTransform->m_Transform.GetTranslation().ToVector3f() - args.obj->tf->m_Transform.GetTranslation().ToVector3f(), args.dt);
			if (shootTimer <= 0.f)
			{
				SpawnBullet(args);
				shootTimer = rof;
			}
		}
	}
}

ZMasher::Vector3f AISystem::Seek(const SteeringArgs& args)
{
	ZMasher::Vector3f steering = args.m_ToTarget;
	steering.Normalize();
	steering*=args.m_Type->m_MaxSpeed;
	return steering;
}

ZMasher::Vector3f AISystem::Arrive(const SteeringArgs& args)
{
	ZMasher::Vector3f steering = args.m_ToTarget.GetNormal();
	
	float speed = (args.m_Type->m_MaxSpeed * args.m_ToTarget.Length())/args.m_Type->m_ArrivingDist;
	return steering*speed;
}

const float global_bullet_speed = 300.f;
void AISystem::SpawnBullet(const AIBehaviourArgs & args)
{
	GameObject bullet = GameObjectManager::Instance()->CreateGameObject();
	ZMasher::Matrix44f bulletTransform = args.obj->tf->m_Transform;
	bulletTransform.SetTranslation(ZMasher::Vector4f(args.obj->tf->m_Transform.GetTranslation()));
	bulletTransform.SetTranslation(bulletTransform.GetTranslation() + args.obj->tf->m_Transform.GetVectorForward() * 7);
	GameObjectManager::Instance()->TransformManager()->AddComponent(bullet, bulletTransform);
	GameObjectManager::Instance()->BulletCompManager()->AddComponent(bullet, 1.f, 1337, 3);
	GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eTurretBullet, 2, bullet, g_TestCallBack);
	GameObjectManager::Instance()->MomentumCompManager()->AddComponent(bullet, 10, (args.obj->tf->m_Transform.GetVectorForward()).ToVector3f() * global_bullet_speed);
} 


void AISystem::AddNewZoldier()
{
}

void AISystem::ClampMaxSpeed(ZMasher::Vector3f& steering, const class AIType* type)
{
	if (steering.Length() > type->m_MaxSpeed)
	{
		steering /= steering.Length();
		steering *= type->m_MaxSpeed;
	}
}

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
void AISystem::FaceDirection(ZMasher::Matrix44f& transform, const ZMasher::Vector3f& direction, const float dt)
{
	ZMasher::Vector4f origin = transform.GetVectorForward();
	ZMasher::Vector4f target = ZMasher::Vector4f(direction, 0);
	origin.y = 0;//the only angle we're interested in is that in the xz plane, remove the other two
	origin.w = 0;

	target.y = 0;
	origin.Normalize();
	target.Normalize();

	const float angle = ZMasher::AngleBetweenVectors(origin, target);
	const ZMasher::Vector4f temp_pos = transform.GetTranslation();
	transform.SetTranslation(ZMasher::Vector4f(0, 0, 0, temp_pos.w));
	transform *= ZMasher::Matrix44f::CreateRotationMatrixY(angle > 0.f ? min(angle, dt*3) : max(angle, -dt*3));
	transform.SetTranslation(temp_pos);
}