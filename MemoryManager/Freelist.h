#pragma once

#include "Allocator.h"

#define FREELIST_TEMPLATE template<typename BaseAllocator, MemSizeType a_alloc_size, MemSizeType max_count>
#define FREELIST_DECL FreeList<typename BaseAllocator, a_alloc_size, max_count>

#define GET_NODE(node_data) reinterpret_cast<FreeListNode*>((MEMSIZETYPE_CAST(node_data) + alloc_size))

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
		
		FreeListNode* m_RootNode;
		FreeListNode* m_LastNode;

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
	Blk FREELIST_DECL::Allocate(MemSizeType)
	{
		if (m_RootNode)
		{
			Blk blk = m_RootNode->data;
			if (m_LastNode == m_RootNode)
			{
				m_LastNode = nullptr;
			}
			m_RootNode = m_RootNode->next;
			--m_Count;
			ASSERT(m_Count <= 1 || m_RootNode != m_LastNode, "Lastnode is pointing at root node!");
			return blk;
		}
		Blk blk = BaseAllocator::Allocate(alloc_size + sizeof(FreeListNode));//suffix added for linked list
		blk.m_Size = alloc_size;//the suffix should be hidden from the user
		GET_NODE(blk.m_Data)->next = nullptr;
		GET_NODE(blk.m_Data)->data = blk;
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
		ASSERT(false, "Can't reallocate!");
	}
	FREELIST_TEMPLATE
	bool FREELIST_DECL::Owns(Blk blk)
	{
		// if it exists in this allocator, it means that something
		// has deallocated it, thus, it's not owned by anything
		// (while it's technically 'owned' by this allocator, this
		// allocator is technically not an allocator, and more of 
		// an allocator helper)
		FreeListNode* temp_node = m_RootNode;
		while (temp_node)
		{
			if (temp_node->data.m_Data == blk.m_Data)
			{
				return false;
			}
			temp_node = temp_node->next;
		}
		return BaseAllocator::Owns(blk);
	}
	FREELIST_TEMPLATE
	void FREELIST_DECL::Deallocate(Blk blk)
	{
		if (m_Count < max_count)
		{
			return AddBlk(blk);
		}
		BaseAllocator::Deallocate(blk);
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
		++m_Count;
		ASSERT(blk.m_Size <= (alloc_size), "Freelist: can't add blk, invalid size");
		ASSERT(BaseAllocator::Owns(blk), "Freelist: Trying to add memory that isn't owned by its BaseAllocator");
		if (m_RootNode == nullptr)
		{
			m_RootNode = GET_NODE(blk.m_Data);//the last space is saved for linked list
			m_RootNode->next = nullptr;
			m_RootNode->data = blk;
			m_LastNode = m_RootNode;
			return;
		}
		ASSERT(m_LastNode->next == nullptr, "Freelist: Something has tampered with memory out of bounds!");
		m_LastNode->next = GET_NODE(blk.m_Data);
		m_LastNode = m_LastNode->next;
		m_LastNode->next = nullptr;
		m_LastNode->data = blk;
		ASSERT(m_LastNode != m_RootNode, "Freelist: Last pointer is pointing at rootnode!");
	}
}