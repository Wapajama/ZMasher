#pragma once

#include <Allocator.h>
#define BUCKETIZER_TEMPLATE template <typename AllocatorType, MemSizeType min, MemSizeType max, MemSizeType step>
#define BUCKETIZER_DECL Bucketizer<AllocatorType, min, max, step>

namespace ZMasher
{

	BUCKETIZER_TEMPLATE
	class Bucketizer
		: public AllocatorType
	{
	public:
		Bucketizer();
		~Bucketizer();

		BASE_ALLOCATOR_OVERRIDE_ALL

	private:

		static_assert(min < max, "MinSize must be smaller than MaxSize");
		static_assert((max - min + 1) % step == 0, "Incorrect ranges or step size!");

#define BUCKETIZER_N_BUCKETS ((max - (min+(min%step))) / step) + 1
#define BUCKETIZER_MIN_SIZE min > step ? min+(min%step) : step

		AllocatorType m_Allocators[BUCKETIZER_N_BUCKETS];

		const MemSizeType m_NoOfBuckets;

		inline const MemSizeType GetAllocatorIndex(const MemSizeType size)
		{
			MemSizeType allocator_min_size = BUCKETIZER_MIN_SIZE;
			for (MemSizeType i = 0; i < BUCKETIZER_N_BUCKETS; i++)
			{
				if ((allocator_min_size+step*i) >= size)
				{
					MemSizeType temp = allocator_min_size+step*i;
					return (temp-min)/step;
				}
			}
			//ERROR!!!
			return (max-min)/step;
		}

		inline AllocatorType& GetAllocator(const MemSizeType size)
		{
			if (!GoodSize(size))
			{
				ASSERT(false, "Bucketizer: invalid size!");
			}
			return m_Allocators[GetAllocatorIndex(size)];
		}
	};

	BUCKETIZER_TEMPLATE
	BUCKETIZER_DECL::Bucketizer()
		: m_NoOfBuckets(BUCKETIZER_N_BUCKETS)
	{
		//MDFINAE
		MemSizeType allocator_min_size = BUCKETIZER_MIN_SIZE;
		for (char i = 0; i < BUCKETIZER_N_BUCKETS; ++i)
		{
			//m_Allocators[i].Init(min + step*i);
			m_Allocators[i].Init(allocator_min_size + step*i);
		}
	}
	BUCKETIZER_TEMPLATE
	BUCKETIZER_DECL::~Bucketizer()
	{
		for (char i = 0; i < BUCKETIZER_N_BUCKETS; ++i)
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
		if (GoodSize(blk.m_Size + delta))
		{
			GetAllocator(blk.m_Size).Deallocate(blk);
			blk = GetAllocator(GetAllocatorIndex(blk.m_Size + delta)).Allocate(blk.m_Size + delta);
			return true;
		}
		return false;
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
		if (GoodSize(size))
		{
			GetAllocator(blk.m_Size).Deallocate(blk);
			blk = GetAllocator(GetAllocatorIndex(size)).Allocate(size);
		}
		else
		{
			ASSERT(false, "Bucketizer: Can't reallocate!");
		}
	}
	BUCKETIZER_TEMPLATE
	bool BUCKETIZER_DECL::Owns(Blk blk)
	{
		return GetAllocator(blk.m_Size).Owns(blk);
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::Deallocate(Blk blk)
	{
		GetAllocator(blk.m_Size).Deallocate(blk);
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::DeallocateAligned(Blk blk)
	{
		//NOT IMPLEMENTED
	}
	BUCKETIZER_TEMPLATE
	void BUCKETIZER_DECL::DeallocateAll()
	{
		for (char i = 0; i < BUCKETIZER_N_BUCKETS; i++)
		{
			m_Allocators[i].DeallocateAll();
		}
	}

}