#include "MemoryManager.h"
#include <malloc.h>
#include <vcruntime_new.h>

#include <iostream>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>



namespace ZMasher
{

	MemoryManager::MemoryManager()
	{
		//explicit malloc in here, because we've overridden the new operator to here so 
		//it will enter "Allocate" before we even finished this constructor
		MainMemoryAllocator* data = reinterpret_cast<MainMemoryAllocator*>(malloc(sizeof(MainMemoryAllocator)));
		m_Allocator = new(data) MainMemoryAllocator();
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
		return m_Allocator->Allocate(size).m_Data;
	}
	void MemoryManager::Free(void* data_ptr)
	{
		if (data_ptr == nullptr)
		{
			return;
		}
		m_Allocator->Deallocate({ data_ptr, -1 });
	}

}