#pragma once
#include "ComponentCollection.h"
#include <TransformComponentManager.h>

class TransformCollection :
	public ComponentCollection
{
public:
	TransformCollection(const ComponentGroup<TransformComponent>& transformGroup, const int count);
	~TransformCollection();

	inline virtual ComponentCollectionType GetType()const override
	{ return ComponentCollectionType::TransformComponent; };

	bool Iterate()override;

protected:

	inline bool IterationResults(bool result1, bool result2) 
	{
		ASSERT(result1 == result2, "Iteration results are not matching!");
		return result1 && result2;
	};

private:
	ComponentGroup<TransformComponent> m_TransformGroup;

};

