#include "AIGroup.h"
#include <GameObjectManager.h>
#include <ZMModelFactory.h>
#include <ZMUtils/Utility/ZMasherUtilities.h>

extern void(*g_TestCallBack)(CollCallbackArgs);

AIGroup::AIGroup(const int size) :
	m_AIObjects(size),
	m_CurrentAIObject(0)
{
}


AIGroup::~AIGroup()
{
}

bool AIGroup::Iterate()
{
	++m_CurrentAIObject;
	if (m_CurrentAIObject >= m_AIObjects.Size())
	{
		m_CurrentAIObject = 0;
		return false;
	}

	return true;
}

const float ai_range = 300.f;
void AIGroup::Allocate(const int size, void* args)
{
	AIObjectArgs* ai_args = nullptr;

	if (args)
	{
		ai_args = (AIObjectArgs*)args;
	}
	else
	{
		return;
	}

	//gFirs
	//	meshFirst
	//	aiFirst,
	//	sphereFir
	//	momFirst,

	meshFirst = GameObjectManager::Instance()->MeshCompManager()->Count();
	aiFirst = GameObjectManager::Instance()->AICompManager()->Count();
	sphereFirst = GameObjectManager::Instance()->SphereCollisionCompManager()->Count();
	momFirst = GameObjectManager::Instance()->MomentumCompManager()->Count();

	for (int i = 0; i < size; i++)
	{
		GameObject new_object = GameObjectManager::Instance()->CreateGameObject();
		if (i == 0)
		{
			gFirst = new_object;
		}
		if (i == size-1)
		{
			gLast = new_object;
		}
		ZMasher::Vector4f position(0, 1, 0.f, 1.f);
		ZMasher::Matrix44f transform = ZMasher::Matrix44f::Identity();
		transform.SetTranslation(position + ZMasher::Vector4f(ZMasher::GetRandomFloat(-ai_range, ai_range), 0, ZMasher::GetRandomFloat(-ai_range, ai_range), 0.f));

		TransformComponent* tf = GameObjectManager::Instance()->TransformManager()->AddComponent(new_object, transform);
		ModelComponent* m = GameObjectManager::Instance()->MeshCompManager()->AddComponent(new_object, ZMModelFactory::Instance()->LoadModelInstance(ai_args->modelName.c_str()));
		SphereCollisionComponent* sp = GameObjectManager::Instance()->SphereCollisionCompManager()->AddComponent(eCOLLISIONTYPE::eSphere,5, new_object, g_TestCallBack);
		MomentumComponent* mo = GameObjectManager::Instance()->MomentumCompManager()->AddComponent(new_object, 4);
		AIComponent* ai = GameObjectManager::Instance()->AICompManager()->AddComponent(new_object, eAIType::ZOLDIER);
		ai->m_TargetPos = ai_args->targetPos;
		//CollisionQuery* q = GameObjectManager::Instance()->GetCollisionSystem()->CreateQuery(eCOLLISIONTYPE::eSphere, new_object, ai_args->radius * 20, transform.GetTranslation().ToVector3f());
		m_AIObjects.Add({ tf, m, ai, sp, mo, new_object });
	}

	meshLast = GameObjectManager::Instance()->MeshCompManager()->Count()-1;
	aiLast = GameObjectManager::Instance()->AICompManager()->Count()-1;
	sphereLast = GameObjectManager::Instance()->SphereCollisionCompManager()->Count()-1;
	momLast = GameObjectManager::Instance()->MomentumCompManager()->Count()-1;
}