#pragma once
#include <Utility\ZMSingleton.h>
#include "Mallocator.h"
#include "StackAllocator.h"

namespace ZMasher
{
	typedef StackAllocator<1024*16, Mallocator> MainMemoryAllocator;
	class MemoryManager
		:public ZMSingleton<MemoryManager>
	{
	public:
		
		void* Allocate(size_t size);
		void Free(void* data_ptr);
		static void CreateMemoryManager();
	private:
		friend class ZMSingleton<MemoryManager>;
		MemoryManager();
		~MemoryManager();
		//Mallocator* m_Mallocator;
		Allocator* m_Allocator;
	};

}