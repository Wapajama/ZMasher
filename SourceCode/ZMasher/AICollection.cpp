#include "AICollection.h"



AICollection::AICollection(		
		const ComponentGroup<AIComponent>& aiGroup,
		const ComponentGroup<SphereCollisionComponent>& sphereGroup,
		const ComponentGroup<MomentumComponent>& momentumGroup,
		const ComponentGroup<ModelComponent>& modelGroup, 
		const ComponentGroup<TransformComponent>& transformGroup,
		const int count)
	: CollisionMeshCollection(sphereGroup, momentumGroup, modelGroup, transformGroup, count)
	, m_AIGroup(aiGroup)

{
}


AICollection::~AICollection()
{
}

bool AICollection::Iterate()
{
	return IterationResults(CollisionMeshCollection::Iterate(), m_AIGroup.Iterate());
}