#pragma once

//Fast access, can't grow, very small volumes of data
#define ASSERT

#define STACK_ARRAY_TEMPLATE template<typename Type, int size, typename index_type>
#define STACK_ARRAY_DECL StackArray<Type, size, index_type>

template<typename Type, int size, typename index_type = unsigned short>
class StackArray
{
public:
	StackArray();
	~StackArray();

	inline void Add(const Type& element);
	__forceinline Type& operator[](index_type index);
	__forceinline const Type& operator[](index_type index)const;
	__forceinline Type& GetLast();
	__forceinline const Type& GetLast()const;

	inline void Remove(index_type index);
	inline void RemoveCyclic(index_type index);

	__forceinline index_type Count()const;
	
	__forceinline void RemoveLast();

private:

	Type m_Data[size];
	index_type m_CurrentSize;
	const index_type m_MaxCount;
	inline bool OutOfBounds(index_type index);
};

STACK_ARRAY_TEMPLATE
STACK_ARRAY_DECL::StackArray():m_MaxCount(size)
{
	m_CurrentSize = 0;
}

STACK_ARRAY_TEMPLATE
STACK_ARRAY_DECL::~StackArray()
{
}

STACK_ARRAY_TEMPLATE
void STACK_ARRAY_DECL::Add(const Type& element)
{
	if (m_CurrentSize >= m_MaxCount)//can't grow
	{
		ASSERT
	}
	m_Data[m_CurrentSize] = element;
	++m_CurrentSize;
}

STACK_ARRAY_TEMPLATE
Type& STACK_ARRAY_DECL::operator[](index_type index)
{
	if (OutOfBounds(index))
	{
		ASSERT
	}
	return m_Data[index];
}

STACK_ARRAY_TEMPLATE
const Type& STACK_ARRAY_DECL::operator[](index_type index)const
{
	if (OutOfBounds(index))
	{
		ASSERT
	}
	return m_Data[index];
}

STACK_ARRAY_TEMPLATE
index_type STACK_ARRAY_DECL::Count()const
{
	return m_CurrentSize;
}

STACK_ARRAY_TEMPLATE
inline bool STACK_ARRAY_DECL::OutOfBounds(index_type index)
{
	return  index < 0 ||
		index >= m_CurrentSize;
}

STACK_ARRAY_TEMPLATE
void STACK_ARRAY_DECL::Remove(index_type index)
{
	if (OutOfBounds(index))
	{
		ASSERT
	}
	for (index_type i = index; i < m_CurrentSize; ++i)
	{
		if (i == m_CurrentSize - 1 )
		{
			break;
		}
		m_Data[i] = m_Data[i + 1];
	}
	--m_CurrentSize;
}

STACK_ARRAY_TEMPLATE
void STACK_ARRAY_DECL::RemoveCyclic(index_type index)
{
	if (OutOfBounds(index))
	{
		ASSERT
	}
	m_Data[index] = m_Data[m_CurrentSize - 1];
	--m_CurrentSize;
}

STACK_ARRAY_TEMPLATE
__forceinline Type& STACK_ARRAY_DECL::GetLast()
{
	return (*this)[m_CurrentSize - 1];
}

STACK_ARRAY_TEMPLATE
__forceinline const Type& STACK_ARRAY_DECL::GetLast()const
{
	return (*this)[m_CurrentSize - 1];
}

STACK_ARRAY_TEMPLATE
inline void STACK_ARRAY_DECL::RemoveLast()
{
	--m_CurrentSize;
}