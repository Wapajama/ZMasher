#pragma once
#include <Allocator.h>

#define SHARED_ALLOCATOR_TEMPL template<void** shared_allocator>
#define SHARED_ALLOCATOR_DECL SharedAllocator<shared_allocator>

namespace ZMasher
{
	class Allocator;
	SHARED_ALLOCATOR_TEMPL
	class SharedAllocator
		: public Allocator
	{
	public:
		SharedAllocator();
		~SharedAllocator();

		BASE_ALLOCATOR_OVERRIDE_ALL
	private:
		inline Allocator* GetAllocator() { return *m_Allocator;}
		Allocator** m_Allocator;
	};
	SHARED_ALLOCATOR_TEMPL
	SHARED_ALLOCATOR_DECL::SharedAllocator()
		: m_Allocator(reinterpret_cast<Allocator**>(shared_allocator))
	{
		ASSERT(GetAllocator() != nullptr, "SharedAllocator: Allocator can't be nullptr!");
	}
	SHARED_ALLOCATOR_TEMPL
	SHARED_ALLOCATOR_DECL::~SharedAllocator()
	{
	}
	SHARED_ALLOCATOR_TEMPL
	bool SHARED_ALLOCATOR_DECL::GoodSize(MemSizeType size)
	{
		return GetAllocator()->GoodSize(size);
	}
	SHARED_ALLOCATOR_TEMPL
	Blk SHARED_ALLOCATOR_DECL::Allocate(MemSizeType size)
	{
		return GetAllocator()->Allocate(size);
	}
	SHARED_ALLOCATOR_TEMPL
	Blk SHARED_ALLOCATOR_DECL::AllocateAll()
	{
		return GetAllocator()->AllocateAll();
	}
	SHARED_ALLOCATOR_TEMPL
	Blk SHARED_ALLOCATOR_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		return AllocateAligned(size, alignment);
	}
	SHARED_ALLOCATOR_TEMPL
	Blk SHARED_ALLOCATOR_DECL::AllocateAllAligned(MemSizeType alignment)
	{
		return GetAllocator()->AllocateAllAligned(alignment);
	}
	SHARED_ALLOCATOR_TEMPL
	bool SHARED_ALLOCATOR_DECL::Expand(Blk& blk, MemSizeType delta)
	{
		return GetAllocator()->Expand(blk, delta);
	}
	SHARED_ALLOCATOR_TEMPL
	void SHARED_ALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
		if (blk.m_Size == size)
		{
			return;
		}
		return GetAllocator()->Reallocate(blk, size);
	}
	SHARED_ALLOCATOR_TEMPL
	bool SHARED_ALLOCATOR_DECL::Owns(Blk blk)
	{
		return GetAllocator()->Owns(blk);
	}
	SHARED_ALLOCATOR_TEMPL
	void SHARED_ALLOCATOR_DECL::Deallocate(Blk blk)
	{
		return GetAllocator()->Deallocate(blk);
	}
	SHARED_ALLOCATOR_TEMPL
	void SHARED_ALLOCATOR_DECL::DeallocateAligned(Blk blk)
	{
		return GetAllocator()->DeallocateAligned(blk);
	}
	SHARED_ALLOCATOR_TEMPL
	void SHARED_ALLOCATOR_DECL::DeallocateAll()
	{
		return GetAllocator()->DeallocateAll();
	}
}