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

	MemoryManager::MemoryManager():
		m_Allocator(nullptr),
		m_ForceMalloc(false),
		m_AllocatedMallocSize(0)
	{
		//explicit malloc in here, because we've overridden the new operator to here so 
		//it will enter "Allocate" before we even finished this constructor
		MainMemoryAllocator* data = reinterpret_cast<MainMemoryAllocator*>(this->Malloc(sizeof(MainMemoryAllocator)));
		m_Allocator = new(data) MainMemoryAllocator();
		//reinterpret_cast<MainMemoryAllocator*>(m_Allocator)->Init();


		//Segregator<1024, Mallocator, StackAllocator<1024, Mallocator>> test;
	}
	MemoryManager::~MemoryManager()
	{
		m_Allocator->~Allocator();
		free(m_Allocator);
	}

	void* MemoryManager::Allocate(size_t size)
	{
		if (size <= 0)
		{
			return nullptr;
		}
		if (m_Allocator == nullptr ||
			m_ForceMalloc)
		{
			return this->Malloc(size);
		}
		m_ForceMalloc = true;
		void* data = m_Allocator->Allocate(size).m_Data;
		m_ForceMalloc = false;
		return data;
	}
	void* MemoryManager::Reallocate(void* data, size_t size)
	{
		if (data == nullptr && 
			size <= 0)
		{
			return nullptr;
		}
		if (m_ForceMalloc)
		{
			return realloc(data, size);
		}
		if (data == nullptr)
		{
			void* new_data = Allocate(size);
			return new_data;
		}

		Blk blk = {data, -1};
		m_ForceMalloc = true;
		m_Allocator->Reallocate(blk, size);
		m_ForceMalloc = false;
		return blk.m_Data;
	}
	void MemoryManager::Free(void* data_ptr)
	{
		if (data_ptr == nullptr)
		{
			return;
		}
		if (m_Allocator == nullptr ||
			m_ForceMalloc)
		{

			return FreeMalloc(data_ptr);
		}
		m_ForceMalloc = true;
		m_Allocator->Deallocate({ data_ptr, -1 });
		m_ForceMalloc = false;
	}

	void* MemoryManager::Malloc(size_t size)
	{
		m_AllocatedMallocSize += size;
		return malloc(size);
	}
	void MemoryManager::FreeMalloc(void* data)
	{
		free(data);
	}
}