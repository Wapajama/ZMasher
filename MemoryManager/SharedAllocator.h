#pragma once
#include <Allocator.h>

namespace ZMasher
{
	class SharedAllocator
		: public Allocator
	{
	public:
		SharedAllocator(Allocator* allocator);
		~SharedAllocator();

		BASE_ALLOCATOR_OVERRIDE_ALL
	private:
		Allocator* m_Allocator;
	};

	SharedAllocator::SharedAllocator(Allocator* allocator)
		: m_Allocator(allocator)
	{
		ASSERT(m_Allocator != nullptr, "SharedAllocator: Allocator can't be nullptr!");
	}
	SharedAllocator::~SharedAllocator()
	{
	}
	bool SharedAllocator::GoodSize(MemSizeType size)
	{
		return m_Allocator->GoodSize(size);
	}
	Blk SharedAllocator::Allocate(MemSizeType size)
	{
		return m_Allocator->Allocate(size);
	}
	Blk SharedAllocator::AllocateAll()
	{
		return m_Allocator->AllocateAll();
	}
	Blk SharedAllocator::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		return AllocateAligned(size, alignment);
	}
	Blk SharedAllocator::AllocateAllAligned(MemSizeType alignment)
	{
		return m_Allocator->AllocateAllAligned(alignment);
	}
	bool SharedAllocator::Expand(Blk& blk, MemSizeType delta)
	{
		return m_Allocator->Expand(blk, delta);
	}
	void SharedAllocator::Reallocate(Blk& blk, MemSizeType size)
	{
		return m_Allocator->Reallocate(blk, size);
	}
	bool SharedAllocator::Owns(Blk blk)
	{
		return m_Allocator->Owns(blk);
	}
	void SharedAllocator::Deallocate(Blk blk)
	{
		return m_Allocator->Deallocate(blk);
	}
	void SharedAllocator::DeallocateAligned(Blk blk)
	{
		return m_Allocator->DeallocateAligned(blk);
	}
	void SharedAllocator::DeallocateAll()
	{
		return m_Allocator->DeallocateAll();
	}
}