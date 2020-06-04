#pragma once
#include "MeshCollection.h"
#include "SphereCollisionComponentManager.h"
#include "MomentumComponentManager.h"
class CollisionMeshCollection :
	public MeshCollection
{
public:
	CollisionMeshCollection(
		const ComponentGroup<SphereCollisionComponent>& sphereGroup,
		const ComponentGroup<MomentumComponent>& momentumGroup,
		const ComponentGroup<ModelComponent>& modelGroup,
		const ComponentGroup<TransformComponent>& transformGroup,
		const int count
	);
	~CollisionMeshCollection();

	inline virtual ComponentCollectionType GetType()const override
	{
		return RETURN_COLLECTION_TYPE(MeshCollection::GetType(), ComponentCollectionType::CollisionComponent);
	}

	bool Iterate()override;

	inline SphereCollisionComponent& CurrentSphere() { return m_SphereGroup.Current(); }
	inline MomentumComponent& CurrentMomentum() { return m_MomentumGroup.Current(); }

private:

	ComponentGroup<SphereCollisionComponent> m_SphereGroup;
	ComponentGroup<MomentumComponent> m_MomentumGroup;

};