#pragma once
#include "ComponentManager.h"
#include <DataStructures\GrowArray.h>
#include <Math\ZMMatrix44.h>
#include <ZMasher\GameObject.h>
#include <Time/Timer.h>
#include <GlobalIncludes/project_defines.h>

struct TransformComponent
{
	TransformComponent() {}
	TransformComponent(GameObject game_object, const ZMasher::Matrix44f& transform) :
		m_GameObject(game_object),
		m_Transform(transform)//implement move semantics? std::move
	{}
	GameObject m_GameObject;
	ZMasher::Matrix44f m_Transform;
};

struct TransformIndexPair
{
	GameObject game_object;
	int index;//index in the array in TransformComponentManager
};

class TransformComparer
	: public ZMasher::BSTComparator<TransformIndexPair>
{
public:
	bool LessThan(const TransformIndexPair& one,const TransformIndexPair& two)const override
	{
		return one.game_object.m_ID < two.game_object.m_ID;
	}
	bool GreaterThan(const TransformIndexPair& one,const TransformIndexPair& two)const override
	{
		return one.game_object.m_ID > two.game_object.m_ID;
	}
	bool Equals(const TransformIndexPair& one,const TransformIndexPair& two)const override
	{
		return one.game_object == two.game_object;
	}

};

class TransformComponentManager :
	public ComponentManager<
		TransformComponent>
{
	friend class BulletSystem;
public:
	TransformComponentManager();
	~TransformComponentManager();

	bool Init()override;
	void Destroy()override;
	bool Update()override;

	TransformComponent* AddComponent(GameObject game_object, const ZMasher::Matrix44f& transform);// Will return false if invalid input, eg already exists, invalid transform etc
	
	ZMasher::Matrix44f* GetTransform(GameObject game_object);

	//inline void UpdateTranslation(GameObject game_object);

	inline ZMasher::Vector4f GetTranslation(GameObject game_object);

	// GrowArray<ZMasher::Vector4f, int, MAX_GAME_OBJS> m_Translations;

	//GrowArray<float, int, MAX_GAME_OBJS> m_TranslationsX;
	//GrowArray<float, int, MAX_GAME_OBJS> m_TranslationsY;
	//GrowArray<float, int, MAX_GAME_OBJS> m_TranslationsZ;

private:
#ifdef BENCHMARK
	Timer m_GetTransformTimeStamp;
#endif // BENCHMARK
};
//
//inline void TransformComponentManager::UpdateTranslation(GameObject game_object)
//{
//	this->GetTranslation(game_object) = this->GetTransform(game_object)->GetTranslation();
//}

inline ZMasher::Vector4f TransformComponentManager::GetTranslation(GameObject game_object)
{
	return this->GetTransform(game_object)->GetTranslation();
}
