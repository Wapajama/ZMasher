#pragma once

#include "Allocator.h"

#define FREELIST_TEMPLATE template<typename BaseAllocator, MemSizeType a_alloc_size, MemSizeType max_count>
#define FREELIST_DECL FreeList<typename BaseAllocator, a_alloc_size, max_count>

namespace ZMasher
{
	FREELIST_TEMPLATE
	class FreeList
		: public BaseAllocator
	{
	public:
		FreeList();
		~FreeList();
		
		BASE_ALLOCATOR_OVERRIDE_ALL

		void Init(const MemSizeType allocation_size);

	private:
		void AddBlk(Blk blk);

		struct FreeListNode
		{
			//added as a suffix to the BLK, knowing this we can always extract the blk by simply stepping back to the front of the allocated data
			FreeListNode* next;
			Blk data;
		};
		
		FreeListNode* m_RootNode, * m_LastNode;

		int m_Count;
		MemSizeType alloc_size;//never change unless it's -1 and it hasn't been initialized yet
	};

	FREELIST_TEMPLATE
	FREELIST_DECL::FreeList():
		alloc_size(a_alloc_size),
		m_RootNode(nullptr),
		m_LastNode(nullptr),
		m_Count(0)
	{
	}
	FREELIST_TEMPLATE
	FREELIST_DECL::~FreeList()
	{
		while (m_RootNode)
		{
			BaseAllocator::Deallocate(m_RootNode->data);
			m_RootNode = m_RootNode->next;
		}
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::Init(const MemSizeType allocation_size)
	{
		ASSERT(alloc_size == -1, "FreeList::Init Can't initialize freelist!");
		alloc_size = allocation_size;
	}

	FREELIST_TEMPLATE
	bool FREELIST_DECL::GoodSize(MemSizeType size)
	{
		return size <= alloc_size;
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::Allocate(MemSizeType size)
	{
		if (m_RootNode)
		{
			Blk blk = (*reinterpret_cast<Blk*>( TO_DATA_PTR(MEMSIZETYPE_CAST( m_RootNode - (sizeof(FreeListNode) + alloc_size)))));
			m_RootNode = m_RootNode->next;
			++m_Count;
			return blk;
		}
		Blk blk = BaseAllocator::Allocate(size + sizeof(FreeListNode));//suffix added for linked list
		blk.m_Size = size;//the suffix should be hidden from the user
		return blk;
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::AllocateAll()
	{
		return {nullptr, 0};
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		//NOT IMPLEMENTED
		return Blk();
	}
	FREELIST_TEMPLATE
	Blk FREELIST_DECL::AllocateAllAligned(MemSizeType alignment)
	{
		return Blk();
	}
	FREELIST_TEMPLATE
	bool FREELIST_DECL::Expand(Blk&, MemSizeType)
	{
		//freelists can't expand
		return false;
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
	}
	FREELIST_TEMPLATE
	bool FREELIST_DECL::Owns(Blk blk)
	{
		return BaseAllocator::Owns(blk);
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::Deallocate(Blk blk)
	{
		if (m_Count < max_count)
		{
			return AddBlk(blk);
		}
		ASSERT(false, "Too many deallocations!");
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::DeallocateAll()
	{
		BaseAllocator::DeallocateAll();
		m_RootNode = nullptr;
		m_LastNode = nullptr;
		m_Count = 0;
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::DeallocateAligned(Blk )
	{
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::AddBlk(Blk blk)
	{
		ASSERT(blk.m_Size <= alloc_size, "Freelist: can't add blk, invalid size");
		ASSERT(BaseAllocator::Owns(blk), "Freelist: Trying to add memory that isn't owned by its BaseAllocator");
		if (m_RootNode == nullptr)
		{
			m_RootNode = reinterpret_cast<FreeListNode*>((MEMSIZETYPE_CAST(blk.m_Data) + alloc_size));//the last space is saved for linked list
			m_RootNode->next = nullptr;
			m_LastNode = m_RootNode;
			return;
		}
		FreeListNode* temp = m_LastNode;
		//temp->next should be nullptr right now
		temp->next = reinterpret_cast<FreeListNode*>((MEMSIZETYPE_CAST(blk.m_Data) + alloc_size));//the last space is saved for linked list
		temp->next->next = nullptr;
		m_LastNode = temp->next;
	}
}