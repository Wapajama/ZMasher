#include "TransformCollection.h"



TransformCollection::TransformCollection(const ComponentGroup<TransformComponent>& transformGroup, const int count)
	: ComponentCollection(count)
	, m_TransformGroup(transformGroup)
{
}


TransformCollection::~TransformCollection()
{
}

bool TransformCollection::Iterate()
{
	return IterationResults(ComponentCollection::Iterate(), m_TransformGroup.Iterate());
}