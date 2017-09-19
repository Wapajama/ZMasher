#pragma once

#include "Allocator.h"

#define FREELIST_TEMPLATE template<typename alloc, MemSizeType min, MemSizeType max, MemSizeType alloc_size, MemSizeType max_count>
#define FREELIST_DECL FreeList<typename alloc, min, max, alloc_size, max_count>

namespace ZMasher
{
	FREELIST_TEMPLATE
	class FreeList
		: public Allocator
	{
	public:
		FreeList();
		~FreeList();
		
		BASE_ALLOCATOR_OVERRIDE_ALL

	private:
		void AllocBatch();
		struct FreeListNode
		{
			Blk data;
			FreeListNode* next;
		};
		alloc m_Allocator;
	};

	FREELIST_TEMPLATE
	FREELIST_DECL::FreeList()
	{
	}

	FREELIST_TEMPLATE
	FREELIST_DECL::~FreeList()
	{
	}
	FREELIST_TEMPLATE
	bool FREELIST_DECL::GoodSize(MemSizeType size)
	{

		return  size >= min &&
				size <= max;
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::Allocate(MemSizeType size)
	{
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::AllocateAll()
	{
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::AllocateAligned(MemSizeType size, unsigned alignment)
	{
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::AllocateAllAligned(unsigned alignment)
	{
	}
	FREELIST_TEMPLATE
	bool FREELIST_DECL::Expand(Blk& blk, MemSizeType delta)
	{
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
	}
	FREELIST_TEMPLATE
	bool FREELIST_DECL::Owns(Blk blk)
	{
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::Deallocate(Blk blk)
	{
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::DeallocateAll()
	{
	}

	FREELIST_TEMPLATE
	void FREELIST_DECL::AllocBatch()
	{

	}
}