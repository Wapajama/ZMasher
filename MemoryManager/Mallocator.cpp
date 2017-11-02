#include "Mallocator.h"
#include <stdlib.h>
//#include <vcruntime_new.h>
#include <malloc.h>
#include <Debugging\ZMDebugger.h>
#include <iostream>
namespace ZMasher
{
	MALLOCATOR_TEMPLATE
	MALLOCATOR_DECL::Mallocator()
		: m_Lookup(true)
	{
	}

	MALLOCATOR_TEMPLATE
	MALLOCATOR_DECL::~Mallocator()
	{
	}

	MALLOCATOR_TEMPLATE
	bool MALLOCATOR_DECL::GoodSize(MemSizeType)
	{
		return true;//in malloc, all sizes are "good" sizes ^^
	}
	int MALLOCATOR_DECL::alloc_counter = 0;
	MALLOCATOR_TEMPLATE
	Blk MALLOCATOR_DECL::Allocate(MemSizeType a_size)
	{
		++alloc_counter;
		void* mem_ptr = malloc(a_size);
		//m_Lookup.Add(mem_ptr);
		m_Lookup.Insert({mem_ptr, a_size});
		return {mem_ptr, a_size};
	}
	MALLOCATOR_TEMPLATE
	Blk MALLOCATOR_DECL::AllocateAll()
	{
		return NULL_BLK;//cant allocate all in Mallocator
	}
	MALLOCATOR_TEMPLATE
	Blk MALLOCATOR_DECL::AllocateAligned(MemSizeType a_size, MemSizeType alignment)
	{
		void* mem_ptr = _mm_malloc(a_size, alignment);
		m_Lookup.Insert({mem_ptr, a_size});
		return {mem_ptr, a_size};
	}
	MALLOCATOR_TEMPLATE
	Blk MALLOCATOR_DECL::AllocateAllAligned(MemSizeType)
	{
		return NULL_BLK;//cant allocate all in Mallocator
	}
	MALLOCATOR_TEMPLATE
	bool MALLOCATOR_DECL::Expand(Blk& blk, MemSizeType delta)
	{
		void* new_memblk = realloc(blk.m_Data, blk.m_Size + delta);
		if (new_memblk == nullptr)
		{
			return false;
		}
		blk.m_Data = new_memblk;
		m_Lookup.Insert(blk);
		return true;
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType a_size)
	{
		if (blk.m_Size = a_size)
		{
			return;
		}
		void* new_memblk = realloc(blk.m_Data, a_size);
		m_Lookup.Insert(blk);
		if (new_memblk == nullptr)
		{
			return;
		}
		blk.m_Data = new_memblk;
	}
	MALLOCATOR_TEMPLATE
	bool MALLOCATOR_DECL::Owns(Blk blk)
	{
		return m_Lookup.Find(blk) != nullptr;
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::Deallocate(Blk blk)
	{
		if (m_Lookup.Find(blk) != nullptr)
		{
			m_Lookup.Delete(blk);
			free(blk.m_Data);
		}
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::DeallocateAll()
	{
		while (m_Lookup.Max() != nullptr)
		{
			free(m_Lookup.Max());
			m_Lookup.Delete(m_Lookup.Max());
		}
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::DeallocateAligned(Blk blk)
	{
		while (m_Lookup.Max() != nullptr)
		{
			_mm_free(m_Lookup.Max());
			m_Lookup.Delete(m_Lookup.Max());
		}
	}

}