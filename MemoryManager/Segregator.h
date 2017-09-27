#pragma once
#include "Allocator.h"
#define SEGREGATOR_TEMPL template<int threshold, class SmallAllocator, class LargeAllocator>
#define SEGREGATOR_DECL Segregator<threshold, SmallAllocator, LargeAllocator>

namespace ZMasher
{
	SEGREGATOR_TEMPL
	class Segregator
		:	private SmallAllocator,
			private LargeAllocator
	{
	public:

		BASE_ALLOCATOR_ALL_METHODS

	};

#define CALL_RIGHT_PARENT(Function) if (size <= threshold)\
		{\
		 return SmallAllocator:: Function ;\
		}\
		return LargeAllocator:: Function ;\

	SEGREGATOR_TEMPL
	bool SEGREGATOR_DECL::GoodSize(MemSizeType size)
	{
		CALL_RIGHT_PARENT( GoodSize(size) );
	}
	SEGREGATOR_TEMPL
	Blk SEGREGATOR_DECL::Allocate(MemSizeType size)
	{
		CALL_RIGHT_PARENT( Allocate(size));
	}
	SEGREGATOR_TEMPL
	Blk SEGREGATOR_DECL::AllocateAll()
	{
		return Blk();//doesn't make sense to AllocateAll() here
	}
	SEGREGATOR_TEMPL
	Blk SEGREGATOR_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		CALL_RIGHT_PARENT( AllocateAligned(size, alignment) );
	}
	SEGREGATOR_TEMPL
	Blk SEGREGATOR_DECL::AllocateAllAligned(MemSizeType)
	{
		return Blk();
	}
	SEGREGATOR_TEMPL
	bool SEGREGATOR_DECL::Expand(Blk& blk, MemSizeType delta)
	{
		if (blk.m_Size <= threshold)
		{
			return SmallAllocator::Expand(blk, delta);
		}
		return LargeAllocator::Expand(blk, delta);
	}
	SEGREGATOR_TEMPL
	void SEGREGATOR_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
		if (blk.m_Size <= threshold)
		{
			return SmallAllocator::Reallocate(blk, size);
		}
		return LargeAllocator::Reallocate(blk, size);
	}
	SEGREGATOR_TEMPL
	bool SEGREGATOR_DECL::Owns(Blk blk)
	{
		if (blk.m_Size <= threshold)
		{
			return SmallAllocator::Owns(blk);
		}
		return LargeAllocator::Owns(blk);
	}
	SEGREGATOR_TEMPL
	void SEGREGATOR_DECL::Deallocate(Blk blk)
	{
		if (blk.m_Size <= threshold &&
			SmallAllocator::Owns(blk))
		{
			return SmallAllocator::Deallocate(blk);
		}
		else if (LargeAllocator::Owns(blk))
		{
			LargeAllocator::Deallocate(blk);
		}
	}
	SEGREGATOR_TEMPL
	void SEGREGATOR_DECL::DeallocateAligned(Blk blk)
	{
		if (blk.m_Size <= threshold &&
			SmallAllocator::Owns(blk))
		{
			return SmallAllocator::Deallocate(blk);
		}
		else if (LargeAllocator::Owns(blk))
		{
			LargeAllocator::Deallocate(blk);
		}
	}
	SEGREGATOR_TEMPL
	void SEGREGATOR_DECL::DeallocateAll()
	{
		SmallAllocator::DeallocateAll();
		LargeAllocator::DeallocateAll();
	}

}