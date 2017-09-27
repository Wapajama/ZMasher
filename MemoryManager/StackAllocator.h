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

		void Init();

		BASE_ALLOCATOR_OVERRIDE_ALL

	private:
		Allocator* m_Allocator;
		Blk m_Data;
		MemSizeType m_CurrentSize;//shitty variable
		void* m_Iterator;
		void MoveToAligned(void* & pointer, MemSizeType size);

		struct BlkListNode
		{
			Blk blk;
			BlkListNode* next;
		};

		class BlkListNodeComparator
			: public BSTComparator<BlkListNode>
		{
		public:
			bool LessThan(const BlkListNode& one, const BlkListNode& two)const override
			{
				return (one.blk.m_Size) < (two.blk.m_Size);
			}
			bool GreaterThan(const BlkListNode& one, const BlkListNode& two)const override
			{
				return (one.blk.m_Size) > (two.blk.m_Size);
			}
			bool Equals(const BlkListNode& one, const BlkListNode& two)const override
			{
				return (one.blk.m_Size) == (two.blk.m_Size);
			}
		};

		ZMasher::BinarySearchTree<Blk, BlkComparator>* m_BlkLookup;
		ZMasher::BinarySearchTree<BlkListNode, BlkListNodeComparator>* m_FreeList;
		GrowArray<BlkListNode,short, 1024>* m_BlkListNodes;
	};

	template<typename Type, typename Args = void>
	Type* MallocNew()
	{
		Type* data = reinterpret_cast<Type*>(malloc(sizeof(Type)));
		return new(data) Type;
	}

#define MEMSIZETYPE_CAST(var) reinterpret_cast<MemSizeType>(var)
#define TO_DATA_PTR(var) reinterpret_cast<void*>(var)
#define ALLOCATED_SIZE 1024*1024*4

	STACK_ALLOCATOR_TEMPLATE
		STACK_ALLOCATOR_DECL::StackAllocator(MemSizeType a_alignment)
		:Allocator(a_alignment)
	{
		m_CurrentSize = 0;
		m_Allocator = MallocNew<MyAllocator>();

		if (alignment > 1)
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
		m_Allocator->Deallocate(m_Data);
		delete m_FreeList;

	}

	STACK_ALLOCATOR_TEMPLATE
		void STACK_ALLOCATOR_DECL::Init()
	{

	}

	STACK_ALLOCATOR_TEMPLATE
		bool STACK_ALLOCATOR_DECL::GoodSize(MemSizeType size)
	{
		return (size + MEMSIZETYPE_CAST(m_Iterator) < MEMSIZETYPE_CAST(m_Data.m_Data) + m_Data.m_Size);
	}

	STACK_ALLOCATOR_TEMPLATE
		Blk STACK_ALLOCATOR_DECL::Allocate(MemSizeType size)
	{
		ASSERT(false, "IMPLEMENT REMOVE FOR SPLAY TREES");
		//Check the freelist first

		//out of memory, resort to fallback allocator
		if (!GoodSize(size))
		{
			return m_Allocator->Allocate(size);//failed to allocate in this allocator, use backup
		}
		//new stack allocation
		m_CurrentSize += size;
		//printf_s("\nStackAllocator\nm_CurrentSize: %i\nm_Size:%i\nm_Data: %i\nm_Iterator: %i\nm_Iterator - m_Data: %i\nsize: %i\n", 
		//		 m_CurrentSize, m_Data.m_Size,MEMSIZETYPE_CAST( m_Data.m_Data), MEMSIZETYPE_CAST(m_Iterator), MEMSIZETYPE_CAST(m_Iterator) - MEMSIZETYPE_CAST(m_Data.m_Data), size);
		Blk blk;

		blk.m_Data = m_Iterator;
		blk.m_Size = size;

		m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + size);

		return blk;
	}
	STACK_ALLOCATOR_TEMPLATE
		Blk STACK_ALLOCATOR_DECL::AllocateAll()
	{
		Blk blk = Allocate((MEMSIZETYPE_CAST(m_Data.m_Data) + (m_Data.m_Size)) - MEMSIZETYPE_CAST(m_Iterator));

		return blk;
	}
	STACK_ALLOCATOR_TEMPLATE
		Blk STACK_ALLOCATOR_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		MoveToAligned(m_Iterator, alignment);
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
		if (blk.m_Data == TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + blk.m_Size))
		{
			m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) + delta);
			return true;
		}
		return false;

	}
	//undefined for stackallocator atm, must work with freelists. For now, backup allocator
	STACK_ALLOCATOR_TEMPLATE
		void STACK_ALLOCATOR_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
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
		if (Owns(blk))
		{
			void* prev_data = blk.m_Data;
			MemSizeType prev_size = m_BlkLookup->Find(blk)->value.m_Size;
			blk = Allocate(size);
			memcpy(blk.m_Data, prev_data, prev_size);
			return;
		}
		m_Allocator->Reallocate(blk, size);
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

		if (blk.m_Data == TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) - blk.m_Size)) //means that it was the latest allocation
		{
			printf_s("DESTACKOLLATE!!!");

			m_CurrentSize -= blk.m_Size;
			m_Iterator = TO_DATA_PTR(MEMSIZETYPE_CAST(m_Iterator) - blk.m_Size);
		}
		//otherwise this allocator can't deallocate, use a Freelist
		if (Owns(blk))
		{
			auto blk_node = m_BlkLookup->Find(blk);
			BlkListNode tmp ={blk_node->value, nullptr};
			if (m_FreeList->Find(tmp))
			{
				//BlkListNode* node = m_FreeList->Find(tmp);
				//while(node->next != nullptr)
				//{
				//	new 
				//}
			}
			else
			{
				m_FreeList->Insert(tmp);
			}
		}
		
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

		while ((MEMSIZETYPE_CAST(pointer) % ((align))) != 0)
		{
			pointer = TO_DATA_PTR(MEMSIZETYPE_CAST(pointer) + 1);
		}
	}
}
