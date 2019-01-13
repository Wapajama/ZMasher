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
		const ZMasher::Vector3f position = ZMasher::Vector3f(transform_comp->m_Transform.GetTranslation());
		const float length_to_target = (ai_comp->m_TargetPos - position).Length();
		const AIType* ai_type = m_AIMngr->GetAIType(m_AIMngr->m_Components[i].m_Type);

		if (length_to_target < ai_type->m_ArrivedDist)
		{
			transform_comp->m_Transform.SetTranslation(ZMasher::Vector4f( -100, position.y, ZMasher::GetRandomFloat(-100, 100), transform_comp->m_Transform.GetTranslation().w));
			ai_comp->m_TargetPos.z = transform_comp->m_Transform.GetTranslation().z;
			continue;
		}
		SteeringArgs steer_args{ai_comp, ai_type, ai_comp->m_TargetPos - position}; 

		FaceDirection(transform_comp->m_Transform, steer_args.m_ToTarget);

		ZMasher::Vector3f steering(0,0,0);

		if (steer_args.m_ToTarget.Length() < steer_args.m_Type->m_ArrivingDist)
		{
			steering += Arrive(steer_args);
		}
		else
		{
			steering += Seek(steer_args);
		}

		ClampMaxSpeed(steering, ai_type);
		auto momentum = m_MomentumMngr->GetComponent(game_object);
		if (momentum != nullptr)
		{
			momentum->m_Speed = steering;
		}
	}
	return true;
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

void AISystem::AddNewZoldier()
{
	//ZMasher::Vector4f position(0, 1, 0.f, 1.f);
	//ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
	//const float range = 100;
	//for (int i = 0; i < 10; ++i)
	//{
	//	transform.SetTranslation(position + ZMasher::Vector4f(ZMasher::GetRandomFloat(-range, range), 0, ZMasher::GetRandomFloat(-range, range), 0));
	//	if (i%2)
	//	{
	//		transform.RotateY(M_PI / 2);
	//	}
	//	GameObject new_object = m_GameObjectManager.CreateGameObject();
	//	m_GameObjects.Add(new_object);
	//	m_GameObjectManager.TransformManager()->AddComponent(new_object, transform);
	//	m_GameObjectManager.MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance("../../data/dragonfly01/dragonfly01.model"));
	//	m_GameObjectManager.CollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere, 15, new_object, 10);
	//	m_GameObjectManager.AICompManager()->AddComponent(new_object, eAIType::ZOLDIER);
	//	
	//	m_GameObjectManager.AICompManager()->GetComponent(new_object)->m_TargetPos = (ZMasher::Vector3f(200, 1, transform.GetTranslation().z));
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