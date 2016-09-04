#pragma once
#include "GrowArray.h"

#define MEM_CONTAINER_TEMPLATE template <typename DataType, int size, typename index_type = unsigned short>
#define MEM_CONTAINER_DECL MemoryContainer<DataType, size, index_type>
MEM_CONTAINER_TEMPLATE
class MemoryContainer
{
public:
	MemoryContainer();
	~MemoryContainer();

	DataType* GetData();

	void Delete(DataType* adress);
	inline const index_type FreeSpace()const;
private:

	GrowArray<index_type, index_type, size> m_Indexes;
	DataType* m_Data;
};

MEM_CONTAINER_TEMPLATE
MEM_CONTAINER_DECL::MemoryContainer()
{
	m_Data = new DataType[size];
	int m_AllocSize = size;
	for (index_type i = 0; i < size; ++i)
	{
		m_Indexes.Add(size - i - 1);
	}
}

MEM_CONTAINER_TEMPLATE
MEM_CONTAINER_DECL::~MemoryContainer()
{
	delete m_Data;
}

MEM_CONTAINER_TEMPLATE
DataType* MEM_CONTAINER_DECL::GetData()
{
	DataType* data = &m_Data[m_Indexes.GetLast()];
	m_Indexes.RemoveLast();
	return data;
}

MEM_CONTAINER_TEMPLATE
void MEM_CONTAINER_DECL::Delete(DataType* adress)
{
	index_type index = static_cast<index_type>(adress - &m_Data[0]);
	if (index > size ||
		index < 0)
	{
		ASSERT("PUNG");
	}
	m_Indexes.Add(index);
}