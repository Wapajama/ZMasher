#include "MemoryBucket.h"



MemoryBucket::MemoryBucket(const int size) :
	m_Size(size)
{
	m_Data = new char[size];
	m_CurrentData = nullptr;
}


MemoryBucket::~MemoryBucket()
{
	delete[] m_Data;
}
