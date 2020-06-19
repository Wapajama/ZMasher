#pragma once

#include "Allocator.h"

#include <iostream>


#define STACK_ALLOCATOR_TEMPLATE template<MemSizeType a_size, typename BaseAllocator>
#define STACK_ALLOCATOR_DECL StackAllocator<a_size, BaseAllocator>
namespace ZMasher
{
	STACK_ALLOCATOR_TEMPLATE
	class StackAllocator :
		public BaseAllocator
	{
	public:
		StackAllocator(MemSizeType alignment = 1);
		~StackAllocator();

		BASE_ALLOCATOR_OVERRIDE_ALL

	private:
		Blk m_Data;
		MemSizeType m_CurrentSize;
		void* m_Iterator;
		void MoveToAligned(void* & pointer, MemSizeType size);
	};


	STACK_ALLOCATOR_TEMPLATE
	STACK_ALLOCATOR_DECL::StackAllocator(MemSizeType a_alignment)
	{
		m_CurrentSize = 0;

		if (a_alignment > 1)
		{
			m_Data = BaseAllocator::AllocateAligned(a_size, a_alignment);
		}
		else
		{
			m_Data = BaseAllocator::Allocate(a_size);
		}
		m_Iterator = m_Data.m_Data;
	}
	STACK_ALLOCATOR_TEMPLATE
	STACK_ALLOCATOR_DECL::~StackAllocator()
	{
		BaseAllocator::Deallocate(m_Data);
	}
	STACK_ALLOCATOR_TEMPLATE
	bool STACK_ALLOCATOR_DECL::GoodSize(MemSizeType size)
	{
		return (MEMSIZETYPE_CAST(m_Iterator) + size <=
				MEMSIZETYPE_CAST(m_Data.m_Data) + m_Data.m_Size);
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::Allocate(MemSizeType size)
	{
		//out of memory, resort to base allocator
		if (!this->STACK_ALLOCATOR_DECL::GoodSize(size))
		{
			//TODO: put this functionality in Fallbackallocator instead
			return BaseAllocator::Allocate(size);//failed to allocate in this allocator, use backup
		}
		//new stack allocation
		m_CurrentSize += size;
		Blk blk;

		blk.m_Data = m_Iterator;
		blk.m_Size = size;

		m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + size);

		return blk;
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::AllocateAll()
	{
		Blk blk = this->STACK_ALLOCATOR_DECL::Allocate((MEMSIZETYPE_CAST(m_Data.m_Data) + (m_Data.m_Size)) - MEMSIZETYPE_CAST(m_Iterator));
		return blk;
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		MoveToAligned(m_Iterator, alignment);
		return this->STACK_ALLOCATOR_DECL::Allocate(size);
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::AllocateAllAligned(MemSizeType alignment)
	{
		MoveToAligned(m_Iterator, alignment);
		return this->STACK_ALLOCATOR_DECL::AllocateAll();
	}
	STACK_ALLOCATOR_TEMPLATE
	bool STACK_ALLOCATOR_DECL::Expand(Blk& blk, MemSizeType delta)
	{
		//1. Check if we are the latest allocation
		//2. If not, deallocate and reallocate within this allocator //NOT IMPLEMENTED, might be out of scope
		//3. If failed, go with backupallocator //same as above
		if (blk.m_Data == TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + blk.m_Size))
		{
			m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + delta);
			return true;
		}
		return false;
	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
		if (blk.m_Size == size)
		{
			return;
		}
		//1. Best case: simply expand/shrink the current block
		//2. Bad case: something else is allocated after current block, reallocate to another 
		//position in the stackallocator
		//3. Worst case: new allocation can't fit in current allocator, forward to backupallocator

		if (blk.m_Data == TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + blk.m_Size))
		{
			blk.m_Size = size;
			m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(blk.m_Data) + size);
			return;
		}
		if (size > blk.m_Size)
		{
			//false means we simply failed, deallocate and allocate again
			if(this->STACK_ALLOCATOR_DECL::Expand(blk, size - blk.m_Size))
			{
				return;
			}
		}

		//if we're lucky, we have overridden the Deallocate and Allocate functions
		//with better functionality

		void* temp_data = blk.m_Data;
		blk = this->STACK_ALLOCATOR_DECL::Allocate(size);
		if (blk.m_Data)
		{
			memcpy(blk.m_Data, temp_data, size);
		}
		this->STACK_ALLOCATOR_DECL::Deallocate(blk);
		ASSERT(blk.m_Data, "Stackollator, Can't reallocate!");
	}
	STACK_ALLOCATOR_TEMPLATE
	bool STACK_ALLOCATOR_DECL::Owns(Blk blk)
	{
		const bool eq_larger_than_data = (MEMSIZETYPE_CAST(blk.m_Data) >= MEMSIZETYPE_CAST(m_Data.m_Data));
		const bool eq_less_than_iterator = ((MEMSIZETYPE_CAST(blk.m_Data) + blk.m_Size) <= 
			MEMSIZETYPE_CAST(m_Data.m_Data) + (MEMSIZETYPE_CAST(m_Iterator)));
				 

		return	eq_larger_than_data &&
				eq_less_than_iterator;
	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::Deallocate(Blk blk)
	{
		if (!this->STACK_ALLOCATOR_DECL::Owns(blk))
		{
			return BaseAllocator::Deallocate(blk);
		}

		if (blk.m_Data == TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) - blk.m_Size)) //means that it was the latest allocation
		{
			m_CurrentSize -= blk.m_Size;
			m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) - blk.m_Size);
		}
		//otherwise this allocator can't deallocate
	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::DeallocateAll()
	{
		m_Iterator = m_Data.m_Data;
	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::DeallocateAligned(Blk blk)
	{
		//NOT IMPLEMENTED
		this->STACK_ALLOCATOR_DECL::Deallocate(blk);
	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::MoveToAligned(void* & pointer, MemSizeType size)
	{
		int align = 2;
		while (align < size)
		{
			align *= 2;
		}
		align /= 2;

		while ((MEMSIZETYPE_CAST(pointer) % ((align))) != 0)
		{
			pointer = TO_DATA_PTR(MEMSIZETYPE_CAST(pointer) + 1);
		}
	}
}
