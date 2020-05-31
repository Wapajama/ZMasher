#include "ComponentCollection.h"



ComponentCollection::ComponentCollection(const int count)
	: m_Count(count)
	, m_CurrentIndex(0)
{
}


ComponentCollection::~ComponentCollection()
{
}

bool ComponentCollection::Iterate()
{
	return ++m_CurrentIndex >= m_Count;
}