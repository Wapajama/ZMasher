#pragma once

#include <Debugging\ZMDebugger.h>
#include <immintrin.h>
#include <vcruntime_new.h>
#include <malloc.h>

#define GROW_ARRAY_TEMPLATE template<typename Type, typename SizeType = short, SizeType size = 32, SizeType alignment = -1>
#define GROW_ARRAY_DECL GrowArray<Type, SizeType, size, alignment>

GROW_ARRAY_TEMPLATE
class GrowArray
{
public:
	GrowArray(SizeType init_size = size);
	GrowArray(const GrowArray<Type>& copy);
	~GrowArray();

	__forceinline Type& operator[](SizeType index);
	__forceinline Type& operator[](SizeType index)const;

	__forceinline const SizeType Size()const;
	__forceinline const SizeType MaxSize()const;//current allocated size on heap

	__forceinline Type& GetLast();
	__forceinline Type& GetLast()const;

	inline void Add(const Type& value);
	inline void Add(const GROW_ARRAY_DECL& value);
	inline void Add(const Type* array, SizeType size, const bool reverse_order = false);

	void RemoveAll();
	void DeleteAll();

	inline void RemoveCyclic(const SizeType index);
	inline void DeleteCyclic(const SizeType index);

	inline void RemoveOrdered(const SizeType index);
	inline void DeleteOrdered(const SizeType index);

	__forceinline void RemoveLast();

	inline void Resize(const SizeType size, const bool copy_previous = false);

	SizeType Find(const Type& element)const;
	SizeType found_none = -1;

	inline bool InsideBounds(SizeType index)const;

	__forceinline bool Empty();

private:
	Type* m_Data;
	SizeType m_CurrentSize;
	SizeType m_CurrentMaxSize;
};

GROW_ARRAY_TEMPLATE
GROW_ARRAY_DECL::GrowArray(const GrowArray<Type>& copy)
{
	this->Resize(copy.Size());
	for (SizeType i = 0; i < copy.Size(); i++)
	{
		this->Add(copy[i]);
	}
}

GROW_ARRAY_TEMPLATE
GROW_ARRAY_DECL::GrowArray(SizeType init_size) :m_Data(nullptr)
{
	this->Resize(init_size, false);
}

GROW_ARRAY_TEMPLATE
GROW_ARRAY_DECL::~GrowArray()
{
	delete[] m_Data;
}

GROW_ARRAY_TEMPLATE
__forceinline Type& GROW_ARRAY_DECL::operator[](SizeType index)
{
	ASSERT(InsideBounds(index), "GrowArray: Out of bounds!");
	return m_Data[index];
}
GROW_ARRAY_TEMPLATE
__forceinline Type& GROW_ARRAY_DECL::operator[](SizeType index)const
{
	ASSERT(InsideBounds(index), "GrowArray: Out of bounds!");
	return m_Data[index];
}
GROW_ARRAY_TEMPLATE
__forceinline const SizeType GROW_ARRAY_DECL::Size()const
{
	return m_CurrentSize;
}
GROW_ARRAY_TEMPLATE
__forceinline const SizeType GROW_ARRAY_DECL::MaxSize()const
{
	return m_CurrentMaxSize;
}
GROW_ARRAY_TEMPLATE
__forceinline Type& GROW_ARRAY_DECL::GetLast()
{
	return (*this)[m_CurrentSize - 1];
}
GROW_ARRAY_TEMPLATE
__forceinline Type& GROW_ARRAY_DECL::GetLast()const
{
	return (*this)[m_CurrentSize - 1];
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::Add(const Type& value)
{
	++m_CurrentSize;
	if (m_CurrentSize > m_CurrentMaxSize)
	{
		this->Resize(m_CurrentMaxSize * 2, true);
	}
	GetLast() = value;
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::Add(const GROW_ARRAY_DECL& list)
{
	for (SizeType i = 0; i < list.Size(); ++i)
	{
		Add(list[i]);
	}
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::Add(const Type* array, SizeType size, const bool reverse_order)
{
	for (SizeType i = 0; i < size; ++i)
	{
		Add(array[i]);
	}
}
GROW_ARRAY_TEMPLATE
void GROW_ARRAY_DECL::RemoveAll()
{
	m_CurrentSize = 0;
}
GROW_ARRAY_TEMPLATE
void GROW_ARRAY_DECL::DeleteAll()
{
	for (SizeType i = 0; i < m_CurrentSize; ++i)
	{
		delete m_Data[i];
	}
	m_CurrentSize = 0;
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::RemoveCyclic(const SizeType index)
{
	m_Data[index] = GetLast();
	--m_CurrentSize;
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::DeleteCyclic(const SizeType index)
{
	delete m_Data[index];
	RemoveCyclic(index);
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::RemoveOrdered(const SizeType index)
{
	--m_CurrentSize;
	for (SizeType i = index; i < m_CurrentSize; ++i)
	{
		m_Data[i] = m_Data[i + 1];
	}
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::DeleteOrdered(const SizeType index)
{
	delete m_Data[index];
	RemoveOrdered(index);
}
GROW_ARRAY_TEMPLATE
__forceinline void GROW_ARRAY_DECL::RemoveLast()
{
	if (m_CurrentSize == 0)
	{
		ASSERT(true, "GrowArray::RemoveBack(), Can't remove more!");
	}
	--m_CurrentSize;
}
GROW_ARRAY_TEMPLATE
inline void GROW_ARRAY_DECL::Resize(const SizeType size, const bool copy_previous)
{
#ifdef _DEBUG
	if (size == m_CurrentMaxSize)
	{
		if (copy_previous == false)
		{
			DEBUG_MSG("Resizing to exactly same size, consider using \"RemoveAll()\"");
		}
		else
		{
			DEBUG_MSG("Resizing to exactly same size, using copy_previous == true has no effect");
		}
	}
	if (size < m_CurrentSize)
	{
		DEBUG_MSG("Resizing to lower size than current size, loss of data");
	}
#endif

	if (size == 0)
	{
		return;
	}
	m_CurrentMaxSize = size;
	SizeType prev_size = m_CurrentSize;
	m_CurrentSize = 0;
	if (copy_previous == true)
	{
		Type* prev_array = m_Data;

		if (alignment == -1)
		{
			m_Data = new Type[m_CurrentMaxSize];
		}
		else
		{
			void* data = _mm_malloc(sizeof(Type)*m_CurrentMaxSize, alignment);
			m_Data = new(data) Type[m_CurrentMaxSize];
		}

		Add(prev_array, prev_size);
		if (alignment == -1)
		{
			delete[] prev_array;
		}
		else
		{
			_mm_free(prev_array);
		}
	}
	else
	{
		if (alignment == -1)
		{
			delete[] m_Data;
			m_Data = new Type[m_CurrentMaxSize];
		}
		else
		{
			_mm_free(m_Data);
			void* data = _mm_malloc(sizeof(Type)*m_CurrentMaxSize, alignment);
			m_Data = new(data) Type[m_CurrentMaxSize];
		}
	}
}
GROW_ARRAY_TEMPLATE
SizeType GROW_ARRAY_DECL::Find(const Type& element)const
{
	for (SizeType i = 0; i < m_CurrentSize; ++i)
	{
		if (m_Data[i] == element)
		{
			return i;
		}
	}
	return found_none;
}
GROW_ARRAY_TEMPLATE
inline bool GROW_ARRAY_DECL::InsideBounds(SizeType index)const
{
	return
		m_CurrentMaxSize != 0 &&
		index < m_CurrentSize &&
		index >= 0;
}
GROW_ARRAY_TEMPLATE
__forceinline bool GROW_ARRAY_DECL::Empty()
{
	return m_CurrentSize == 0;
}