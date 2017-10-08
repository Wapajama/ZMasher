#pragma once
#include <Utility\ZMSingleton.h>
#include "Mallocator.h"
#include "StackAllocator.h"

namespace ZMasher
{
	typedef StackAllocator<1024*32, Mallocator> MainMemoryAllocator;
	class MEMORY_DLL MemoryManager
	{
	public:
		MemoryManager();
		~MemoryManager();
		
		void* Allocate(size_t size);
		void* Reallocate(void* data, size_t size);
		void Free(void* data_ptr);

		void SetForceMalloc(bool force_malloc){m_ForceMalloc = force_malloc;}
	private:

		void* Malloc(size_t size);
		void FreeMalloc(void* data_ptr);

		Allocator* m_Allocator;
		
		bool m_ForceMalloc;
		size_t m_AllocatedMallocSize;

	};

}

extern void* ZMAlloc(size_t size);
extern void* ZMCalloc(size_t size, size_t val);
extern void  ZMFree(void* pointerToDelete);
extern void* ZMRealloc(void* data, size_t size);

extern MEMORY_DLL ZMasher::MemoryManager g_MemoryManager;
extern ZMasher::MemoryManager* g_Derp;