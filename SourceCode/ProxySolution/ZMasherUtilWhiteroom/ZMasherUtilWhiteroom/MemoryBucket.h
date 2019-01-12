#pragma once

//Class for self-maintained memory management.
//PRO: No pesky heap allocations
//CON: Can't delete individual elements from bucket (new project?)

class MemoryBucket
{
public:
	MemoryBucket(const int size = 1024*128);//size in bytes
	~MemoryBucket();

	template<typename Type>
	Type* GetData();

	inline void Clear();

private:

	char* m_Data;
	char* m_CurrentData;//relative to m_Data, 
						//first allocation is nullptr, next is nullptr + sizeof(Type) and so on

	const int m_Size;

};

template<typename Type>
Type* MemoryBucket::GetData()
{
	Type data = new (int(m_Data) + int(m_CurrentData)) Type();
	m_CurrentData += sizeof(Type) * 8;
	return data;
}