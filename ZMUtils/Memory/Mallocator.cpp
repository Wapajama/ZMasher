#include "Mallocator.h"
#include <stdlib.h>
//#include <vcruntime_new.h>
#include <malloc.h>
#include <Debugging\ZMDebugger.h>

namespace ZMasher
{
	MALLOCATOR_TEMPLATE
	MALLOCATOR_DECL::Mallocator()
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
	MALLOCATOR_TEMPLATE
	Blk MALLOCATOR_DECL::Allocate(MemSizeType a_size)
	{
		void* mem_ptr = malloc(a_size);
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
		return true;
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType a_size)
	{
		void* new_memblk = realloc(blk.m_Data, a_size);
		if (new_memblk == nullptr)
		{
			return;
		}
		blk.m_Data = new_memblk;
	}
	MALLOCATOR_TEMPLATE
	bool MALLOCATOR_DECL::Owns(Blk blk)
	{
		return false;//don't know :p
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::Deallocate(Blk blk)
	{
		free(blk.m_Data);
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::DeallocateAll()
	{
		//
	}
	MALLOCATOR_TEMPLATE
	void MALLOCATOR_DECL::DeallocateAligned(Blk blk)
	{
		_mm_free(blk.m_Data);
	}

}