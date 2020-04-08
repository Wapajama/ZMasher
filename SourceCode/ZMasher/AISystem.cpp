#include "AISystem.h"
#include <ZMasher/AIComponentManager.h>
#include <ZMasher/SphereCollisionComponentManager.h>
#include <ZMasher/MomentumComponentManager.h>
#include <ZMasher/TransformComponentManager.h>
#include <ZMasher/GameObjectManager.h>
#include <ZMUtils\Utility\ZMasherUtilities.h>

AISystem::AISystem(AIComponentManager* ai,
		SphereCollisionComponentManager* sphere_collision,
		MomentumComponentManager* momentum,
		TransformComponentManager* transform)
	: m_AIMngr(ai)
	, m_SphereCollisionMngr(sphere_collision)
	, m_MomentumMngr(momentum)
	, m_TransformMngr(transform)
{
}


AISystem::~AISystem()
{
}

bool AISystem::Init(void*)
{
	m_AIInternalTimeStamp = Profiler::Instance()->AddTask("AISystemInternal");
	return true;
}

const float ai_range = 300.f;

bool AISystem::Simulate(const float dt)
{
	//first try, everyone seeks 0,0,0
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
			HitlerBehaviour({transform_comp, ai_comp, game_object});
		}
		break;
		case eAIType::BASIC_TURRET:
		{
			BasicTurretBehaviour({transform_comp, ai_comp, game_object});
		}
			break;

		default:
			break;
		}
	}
	return true;
}

void AISystem::HitlerBehaviour(const AIBehaviourArgs& args)
{
	const ZMasher::Vector3f position = ZMasher::Vector3f(args.transform_comp->m_Transform.GetTranslation());
	const float length_to_target = (args.ai_comp->m_TargetPos - position).Length();
	const AIType* ai_type = m_AIMngr->GetAIType(args.ai_comp->m_Type);

	if (length_to_target < ai_type->m_ArrivedDist)
	{
		args.transform_comp->m_Transform.SetTranslation(ZMasher::Vector4f(-ai_range, position.y, ZMasher::GetRandomFloat(-ai_range, ai_range), args.transform_comp->m_Transform.GetTranslation().w));
		args.ai_comp->m_TargetPos.z = ai_range * 0.5;
		return;
	}
	SteeringArgs steer_args{ args.ai_comp, ai_type, args.ai_comp->m_TargetPos - position };

	FaceDirection(args.transform_comp->m_Transform, steer_args.m_ToTarget);

	ZMasher::Vector3f steering(0, 0, 0);

	if (steer_args.m_ToTarget.Length() < steer_args.m_Type->m_ArrivingDist)
	{
		steering += Arrive(steer_args);
	} else
	{
		steering += Seek(steer_args);
	}

	ClampMaxSpeed(steering, ai_type);
	MomentumComponent* momentum = m_MomentumMngr->GetComponent(args.game_object);
	if (momentum != nullptr)
	{
		momentum->m_Speed = steering;
	}
}

void AISystem::BasicTurretBehaviour(const AIBehaviourArgs & args)
{

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
	ZMasher::Matrix44f bulletTransform = args.transform_comp->m_Transform;
	bulletTransform.SetTranslation(ZMasher::Vector4f(args.transform_comp->m_Transform.GetTranslation()));
	bulletTransform.SetTranslation(bulletTransform.GetTranslation() + args.transform_comp->m_Transform.GetVectorForward() * 20);
	GameObjectManager::Instance()->TransformManager()->AddComponent(bullet, bulletTransform);
	GameObjectManager::Instance()->BulletCompManager()->AddComponent(bullet, 1.f, 1337, 3);
	//GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 2, bullet, g_TestCallBack);
	GameObjectManager::Instance()->MomentumCompManager()->AddComponent(bullet, 10, (args.transform_comp->m_Transform.GetVectorForward()).ToVector3f() * global_bullet_speed);
} 

//void(*g_TestCallBack)(CollCallbackArgs) = [](CollCallbackArgs args) 
//{
//	//GameObjectManager::Instance()->Destroy(args.a->m_GameObject);
//	//GameObjectManager::Instance()->Destroy(args.b->m_GameObject);
//	return; 
//};

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

void AISystem::FaceDirection(ZMasher::Matrix44f& transform, const ZMasher::Vector3f& direction)
{
	ZMasher::Vector4f origin = transform.GetVectorForward();
	ZMasher::Vector4f target = ZMasher::Vector4f(direction, 0);
	origin.y = 0;//the only angle we're interested in is that in the xz plane, remove the other two
	origin.w = 0;

	target.y = 0;
	origin.Normalize();
	target.Normalize();

	const float angle = acos(min(ZMasher::Dot(origin, target),1));
	if (angle < 0.001)
	{
		return;
	}
	const ZMasher::Vector4f temp_pos = transform.GetTranslation();
	transform.SetTranslation(ZMasher::Vector4f(0, 0, 0, temp_pos.w));
	transform *= ZMasher::Matrix44f::CreateRotationMatrixY(angle);
	transform.SetTranslation(temp_pos);
}