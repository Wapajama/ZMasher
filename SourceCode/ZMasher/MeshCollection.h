#pragma once
#include "TransformCollection.h"
#include <MeshComponentManager.h>
class MeshCollection :
	public TransformCollection
{
public:
	MeshCollection(const ComponentGroup<ModelComponent>& modelGroup,
				   const ComponentGroup<TransformComponent>& transformGroup,
				   const int count);
	~MeshCollection();

	inline virtual ComponentCollectionType GetType()const override
	{
		return RETURN_COLLECTION_TYPE(TransformCollection::GetType(),ComponentCollectionType::MeshComponent);
	}

	bool Iterate()override;

	inline ModelComponent& CurrentMesh() { return m_MeshGroup.Current(); }
private:
	ComponentGroup<ModelComponent> m_MeshGroup;
};

