#pragma once
#include "CollisionMeshCollection.h"
#include "AIComponentManager.h"

class AICollection :
	public CollisionMeshCollection
{
public:
	AICollection(
		const ComponentGroup<AIComponent>& aiGroup,
		const ComponentGroup<SphereCollisionComponent>& sphereGroup,
		const ComponentGroup<MomentumComponent>& momentumGroup,
		const ComponentGroup<ModelComponent>& modelGroup,
		const ComponentGroup<TransformComponent>& transformGroup,
		const int count);
	~AICollection();

	inline virtual ComponentCollectionType GetType()const override 
	{
		return RETURN_COLLECTION_TYPE(CollisionMeshCollection::GetType(), ComponentCollectionType::AIComponent);
	}

	bool Iterate()override;

	inline AIComponent& CurrentAI() { return m_AIGroup.Current(); }

private:
	ComponentGroup<AIComponent> m_AIGroup;

};

