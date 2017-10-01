#pragma once

#include <Allocator.h>
#define BUCKETIZER_TEMPLATE template <typename Allocator, MemSizeType min, MemSizeType max, MemSizeType step>
#define BUCKETIZER_DECL Bucketizer<Allocator, min, max, step>

namespace ZMasher
{

	BUCKETIZER_TEMPLATE
	class Bucketizer
		: public Allocator
	{
	public:
		Bucketizer();
		~Bucketizer();

	private:

		static_assert(min < max, "MinSize must be smaller than MaxSize");
		static_assert((max - min + 1) % step == 0, "Incorrect ranges or step size!");

		constexpr MemSizeType m_NumberOfBuckets = (max - min) / step;

		Allocator m_Allocators[m_NumberOfBuckets];

		inline Allocator& GetAllocator(const MemSizeType size)
		{
			if (!GoodSize(size))
			{
				ASSERT(false, "Bucketizer: invalid size!");
			}
			const MemSizeType index = (size - min)% step;
			return m_Allocators[index];
		}
	};

	BUCKETIZER_TEMPLATE
	BUCKETIZER_DECL::Bucketizer()
	{
		//MDFINAE
		for (char i = 0; i < m_NumberOfBuckets; ++i)
		{
			m_Allocators[i].Init(min + step*i);
		}
	}
	BUCKETIZER_TEMPLATE
	BUCKETIZER_DECL::~Bucketizer()
	{
		for (char i = 0; i < m_NumberOfBuckets; ++i)
		{
			m_Allocators[i].DeallocateAll();
		}
	}
	BUCKETIZER_TEMPLATE
	bool BUCKETIZER_DECL::GoodSize(MemSizeType size)
	{
		return	size >= min && 
				size <= max;
	}
	BUCKETIZER_TEMPLATE
	Blk BUCKETIZER_DECL::Allocate(MemSizeType size)
	{
		return GetAllocator(size).Allocate(size);
	}
	BUCKETIZER_TEMPLATE
	Blk BUCKETIZER_DECL::AllocateAll()
	{
		return Blk();
	}
	BUCKETIZER_TEMPLATE
	Blk BUCKETIZER_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		return Blk();
	}
	BUCKETIZER_TEMPLATE
	Blk BUCKETIZER_DECL::AllocateAllAligned(MemSizeType alignment)
	{
		return Blk();
	}
	BUCKETIZER_TEMPLATE
	bool BUCKETIZER_DECL::Expand(Blk& blk, MemSizeType delta)
	{

	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::Reallocate(Blk& blk, MemSizeType size)
	{

	}
	BUCKETIZER_TEMPLATE
	bool BUCKETIZER_DECL::Owns(Blk blk)
	{
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::Deallocate(Blk blk)
	{
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::DeallocateAligned(Blk blk)
	{
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::DeallocateAll()
	{
	}

}