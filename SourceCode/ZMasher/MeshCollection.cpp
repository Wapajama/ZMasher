#include "MeshCollection.h"

MeshCollection::MeshCollection(
	const ComponentGroup<ModelComponent>& modelGroup,
	const ComponentGroup<TransformComponent>& transformGroup,
	const int count)
	: TransformCollection(transformGroup, count)
	, m_MeshGroup(modelGroup)
{
}


MeshCollection::~MeshCollection()
{
}

bool MeshCollection::Iterate()
{
	return IterationResults(TransformCollection::Iterate(), m_MeshGroup.Iterate());
}