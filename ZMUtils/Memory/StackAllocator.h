#pragma once
#include "Allocator.h"

#include <iostream>

#define STACK_ALLOCATOR_TEMPLATE template<MemSizeType size, typename MyAllocator>
#define STACK_ALLOCATOR_DECL StackAllocator<size, typename MyAllocator>
namespace ZMasher
{
	STACK_ALLOCATOR_TEMPLATE
	class StackAllocator :
		public Allocator
	{
	public:
		StackAllocator(MemSizeType alignment = 1);
		~StackAllocator();

		BASE_ALLOCATOR_OVERRIDE_ALL

	private:
		Allocator* m_Allocator;
		Blk m_Data;
		MemSizeType m_CurrentSize;//shitty variable
		void* m_Iterator;
		void MoveToAligned(void* & pointer, MemSizeType size);
	};

#define MEMSIZETYPE_CAST(var) reinterpret_cast<MemSizeType>(var)
#define TO_DATA_PTR(var) reinterpret_cast<void*>(var)

	STACK_ALLOCATOR_TEMPLATE
	STACK_ALLOCATOR_DECL::StackAllocator(MemSizeType a_alignment)
		:Allocator(a_alignment)
	{
		m_CurrentSize = 0;
		MyAllocator* data = reinterpret_cast<MyAllocator*>(malloc(sizeof(MyAllocator)));
		m_Allocator = new(data) MyAllocator();

		if (alignment> 1)
		{
			m_Data = m_Allocator->AllocateAligned(size, alignment);
		}
		else
		{
			m_Data = m_Allocator->Allocate(size);
		}
		m_Iterator = m_Data.m_Data;
	}

	STACK_ALLOCATOR_TEMPLATE
	STACK_ALLOCATOR_DECL::~StackAllocator()
	{
		//if (alignment > 1)
		{
			m_Allocator->Deallocate(m_Data);//TODO: add function to see if it was aligned?
		}

	}

	STACK_ALLOCATOR_TEMPLATE
	bool STACK_ALLOCATOR_DECL::GoodSize(MemSizeType size)
	{
		return (size + MEMSIZETYPE_CAST(m_Iterator) < MEMSIZETYPE_CAST(m_Data.m_Data) + m_Data.m_Size);
	}

	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::Allocate(MemSizeType size)
	{
		m_CurrentSize += size;
		if (MEMSIZETYPE_CAST(m_Iterator) + size >
			MEMSIZETYPE_CAST(m_Data.m_Data) + m_Data.m_Size)
		{
			std::cout << "StackAllocator \nsize: " << m_Data.m_Size
						<< "\ncurrent size: " << m_CurrentSize << std::endl;
			return m_Allocator->Allocate(size);//failed to allocate in this allocator, use backup
		}
		Blk blk;

		blk.m_Data = m_Iterator;
		blk.m_Size = size;

		m_Iterator= TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + size);

		return blk;
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::AllocateAll()
	{
		Blk blk;

		blk.m_Data = m_Iterator;
		blk.m_Size = (MEMSIZETYPE_CAST(m_Data.m_Data) + (m_Data.m_Size)) - MEMSIZETYPE_CAST(m_Iterator);
		m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Data.m_Data) + m_Data.m_Size);

		return blk;
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		MoveToAligned(m_Iterator,alignment);
		return Allocate(size);
	}
	STACK_ALLOCATOR_TEMPLATE
	Blk STACK_ALLOCATOR_DECL::AllocateAllAligned(MemSizeType alignment)
	{
		MoveToAligned(m_Iterator, alignment);
		return AllocateAll();
	}
	STACK_ALLOCATOR_TEMPLATE
	bool STACK_ALLOCATOR_DECL::Expand(Blk& blk, MemSizeType delta)
	{
		//1. Check if we are the latest allocation
		//2. If not, deallocate and reallocate within this allocator //NOT IMPLEMENTED, might be out of scope
		//3. If failed, go with backupallocator //same as above
		if (blk.m_Data == TO_DATA_PTR( MEMSIZETYPE_CAST(m_Iterator) + blk.m_Size))
		{
			m_Iterator = TO_DATA_PTR( MEMSIZETYPE_CAST(m_Iterator) + delta);
			return true;
		}
		return false;

	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
		//1. Best case: simply expand/shrink the current block
		//2. Bad case: something else is allocated after current block
		//3. Worst case: new allocation can't fit in current allocator, forward to backupallocator
		if (blk.m_Data == TO_DATA_PTR( MEMSIZETYPE_CAST(m_Iterator)+ blk.m_Size))
		{
			blk.m_Size = size;
			m_Iterator =TO_DATA_PTR( MEMSIZETYPE_CAST(blk.m_Data) + size);
			return;
		}
		blk = m_Allocator->Allocate(size);
	}
	STACK_ALLOCATOR_TEMPLATE
	bool STACK_ALLOCATOR_DECL::Owns(Blk blk)
	{
		return MEMSIZETYPE_CAST(m_Data.m_Data) <= MEMSIZETYPE_CAST(blk.m_Data) &&
				MEMSIZETYPE_CAST(m_Data.m_Data) + m_Data.m_Size > MEMSIZETYPE_CAST(blk.m_Data) + blk.m_Size;
	}
	STACK_ALLOCATOR_TEMPLATE
	void STACK_ALLOCATOR_DECL::Deallocate(Blk blk)
	{
		if (blk.m_Data == TO_DATA_PTR( MEMSIZETYPE_CAST(m_Iterator) - blk.m_Size)) //means that it was the latest allocation
		{
			m_CurrentSize -= blk.m_Size;
			m_Iterator = TO_DATA_PTR( MEMSIZETYPE_CAST( m_Iterator) - blk.m_Size);
		}
		//otherwise this allocator can't deallocate, use with a Freelist
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

		while ((align%(MEMSIZETYPE_CAST( pointer))) != 0)
		{
			pointer = TO_DATA_PTR( MEMSIZETYPE_CAST(pointer) + 1);
		}
	}
}
