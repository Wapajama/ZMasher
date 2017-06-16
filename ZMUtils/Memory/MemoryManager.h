#pragma once
#include <Utility\ZMSingleton.h>
#include "Mallocator.h"
#include "StackAllocator.h"

namespace ZMasher
{
	typedef StackAllocator<1024*512, Mallocator> MainMemoryAllocator;
	class MemoryManager
	{
	public:
		MemoryManager();
		~MemoryManager();
		
		void* Allocate(size_t size);
		void Free(void* data_ptr);
	private:

		//Mallocator* m_Mallocator;
		Allocator* m_Allocator;
		
	};

}

extern ZMasher::MemoryManager g_MemoryManager;