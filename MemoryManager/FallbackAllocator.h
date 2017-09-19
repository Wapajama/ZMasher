#pragma once
#include "Allocator.h"

#define FALLBACK_ALLOCATOR_TEMPL template<class P, class F>
#define FALLBACK_ALLOCATOR_DECL FallbackAllocator<class P, class F>

FALLBACK_ALLOCATOR_TEMPL
class FallbackAllocator
	:	private P,
		private F
{
public:

	BASE_ALLOCATOR_OVERRIDE_ALL
};

FALLBACK_ALLOCATOR_TEMPL
bool FALLBACK_ALLOCATOR_DECL::GoodSize(MemSizeType size)
{
	return P::GoodSize(size) || F::GoodSize(size);
}
FALLBACK_ALLOCATOR_TEMPL
Blk FALLBACK_ALLOCATOR_DECL::Allocate(MemSizeType size)
{
	Blk data = P::Allocate(size);
	if (data.data != nullptr)
	{
		return data;
	}
	return F::Allocate(size);
}
FALLBACK_ALLOCATOR_TEMPL
Blk FALLBACK_ALLOCATOR_DECL::AllocateAll()
{
	Blk data = P::AllocateAll(size);
	if (data.data != nullptr)
	{
		return data;
	}
	return F::AllocateAll(size);
}
FALLBACK_ALLOCATOR_TEMPL
Blk FALLBACK_ALLOCATOR_DECL::AllocateAligned(MemSizeType size, unsigned alignment)
{
	Blk data = P::AllocateAll(size);
	if (data.data != nullptr)
	{
		return data;
	}
	return F::AllocateAll(size);
}
FALLBACK_ALLOCATOR_TEMPL
Blk FALLBACK_ALLOCATOR_DECL::AllocateAllAligned(unsigned alignment)
{
	Blk data = P::AllocateAll(size);
	if (data.data != nullptr)
	{
		return data;
	}
	return F::AllocateAll(size);
}
FALLBACK_ALLOCATOR_TEMPL
bool FALLBACK_ALLOCATOR_DECL::Expand(Blk& blk, MemSizeType delta)
{
	if (P::Expand(blk,delta))
	{
		return true;
	}
	return F::Expand(blk,delta);
}
FALLBACK_ALLOCATOR_TEMPL
void FALLBACK_ALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType size)
{
	if (size > blk.size)
	{
		if (!Expand(blk, size))
		{
			blk.data = nullptr;//wont affect the previous pointer
			return;
		}
	}
	Blk tmp = blk;
	P::Reallocate(tmp, size);
	if (tmp.data == nullptr)
	{
		F::Reallocate(blk, size);
	}
}
FALLBACK_ALLOCATOR_TEMPL
bool FALLBACK_ALLOCATOR_DECL::Owns(Blk blk)
{
	return P::Owns(blk) || F::Owns(blk);
}
FALLBACK_ALLOCATOR_TEMPL
void FALLBACK_ALLOCATOR_DECL::Deallocate(Blk blk)
{
	if (P::Owns(blk))
	{
		return P::Deallocate(blk);
	}
	F::Deallocate(blk);
}
FALLBACK_ALLOCATOR_TEMPL
void FALLBACK_ALLOCATOR_DECL::DeallocateAll()
{
	P::DeallocateAll();
	F::DeallocateAll();
}