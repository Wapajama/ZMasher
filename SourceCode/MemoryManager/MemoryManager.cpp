#include "MemoryManager.h"
#include <malloc.h>
#include <vcruntime_new.h>

#include <iostream>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
#include <Debugging\ZMDebugger.h>
#include <Segregator.h>

ZMasher::MemoryManager g_MemoryManager;
namespace ZMasher
{
#define MAIN_ALLOCATOR_DECL MAIN_ALLOCATOR(reinterpret_cast<void**>(&g_shared_allocator))
	MemoryManager::MemoryManager():
		m_Allocator(nullptr),
		m_Lookup(true)
	{
		//explicit malloc in here, because we've overridden the new operator to here so 
		//it will enter "Allocate" before we even finished this constructor
		void* shared_allocator_malloc = this->Malloc(sizeof(SharedFreelistAllocator));
		g_shared_allocator = new(shared_allocator_malloc) SharedFreelistAllocator();
	
		MAIN_ALLOCATOR_DECL* data = reinterpret_cast<MAIN_ALLOCATOR_DECL*>(this->Malloc(sizeof(MAIN_ALLOCATOR_DECL)));
		m_Allocator = reinterpret_cast<Allocator*>( new(data) MAIN_ALLOCATOR_DECL);
	}
	MemoryManager::~MemoryManager()
	{
		FreeMalloc(m_Allocator);
		m_Allocator = nullptr;
	}

	void* MemoryManager::Allocate(size_t size)
	{
		if (size <= 0)
		{
			return nullptr;
		}
		if (m_Allocator == nullptr)
		{
			return nullptr;
		}
		Blk data = m_Allocator->Allocate(size);
		m_Lookup.Insert(data);
		ASSERT(data.m_Data, "Can't allocate!");
		return data.m_Data;
	}
	void* MemoryManager::Reallocate(void* data, size_t size)
	{
		if (m_Allocator == nullptr)
		{
			return data;
		}
		if (data == nullptr &&
			size <= 0)
		{
			return nullptr;
		}
		if (data == nullptr)
		{
			void* new_data = Allocate(size);
			return new_data;
		}
		MemManBlkNode* blk_node = m_Lookup.Find({data, -1});
		if (blk_node == nullptr)
		{
			ASSERT(false, "Can't reallocate non allocated memory!");
			return nullptr;
		}
		Blk blk = blk_node->value;
		if (blk.m_Size == size)
		{
			return data;
		}
		m_Allocator->Reallocate(blk, size);

		if (blk.m_Data != data)
		{
			//means that it has changed position in the allocator
			m_Lookup.Delete(blk_node);
			m_Lookup.Insert(blk);
		}
		else
		{
			blk_node->value = blk;
		}
		ASSERT(blk.m_Data, "Can't reallocate memory!");
		return blk.m_Data;
	}
	void MemoryManager::Free(void* data_ptr)
	{
		if (data_ptr == nullptr)
		{
			return;
		}
		if (m_Allocator == nullptr)
		{
			return;
		}

		MemManBlkNode* blk_node = m_Lookup.Find({data_ptr, -1});
		if (blk_node == nullptr)
		{
			//ASSERT(false, "Can't find pointer to delete!");
			return;
		}
		m_Allocator->Deallocate(blk_node->value);
		m_Lookup.Delete(blk_node);
	}

	void* MemoryManager::Malloc(size_t size)
	{
		return malloc(size);
	}
	void MemoryManager::FreeMalloc(void* data)
	{
		free(data);
	}

	MemoryManager::SharedFreelistAllocator* MemoryManager::g_shared_allocator = nullptr;
}