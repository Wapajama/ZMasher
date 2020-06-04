#include "CollisionMeshCollection.h"



CollisionMeshCollection::CollisionMeshCollection(
		const ComponentGroup<SphereCollisionComponent>& sphereGroup,
		const ComponentGroup<MomentumComponent>& momentumGroup,
		const ComponentGroup<ModelComponent>& modelGroup, 
		const ComponentGroup<TransformComponent>& transformGroup,
		const int count)
	: MeshCollection(modelGroup, transformGroup, count)
	, m_SphereGroup(sphereGroup)
	, m_MomentumGroup(momentumGroup)
{
}


CollisionMeshCollection::~CollisionMeshCollection()
{
}

bool CollisionMeshCollection::Iterate()
{
	return IterationResults(MeshCollection::Iterate(), IterationResults(m_SphereGroup.Iterate(), m_MomentumGroup.Iterate()));
}