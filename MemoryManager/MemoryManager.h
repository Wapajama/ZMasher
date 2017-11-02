#pragma once
#include <Utility\ZMSingleton.h>
#include <StackAllocator.h>
#include <Mallocator.h>
#include <Segregator.h>
#include <FreeBlkList.h>
#include <Freelist.h>
#include <Bucketizer.h>
#include <SharedAllocator.h>

namespace ZMasher
{
	const MemSizeType BlkListStackSize = 1024*1024*8;
	const MemSizeType BlkListBlkSize = 256;
	const MemSizeType loc_max_free_list_count = 4096*4;
	class MEMORY_DLL MemoryManager
	{
	public:
		MemoryManager();
		~MemoryManager();
		
		void* Allocate(size_t size);
		void* Reallocate(void* data, size_t size);
		void Free(void* data_ptr);

	private:

		void* Malloc(size_t size);
		void FreeMalloc(void* data_ptr);

		typedef StackAllocator<1024*1024*4, Mallocator> SharedFreelistAllocator;
		static SharedFreelistAllocator* g_shared_allocator;
		
		#define BUCKETIZER_FREELIST(min, max, step, shared_allocator) Bucketizer<FreeList<SharedAllocator<shared_allocator>,-1, loc_max_free_list_count>, min, max, step>
		#define MAIN_ALLOCATOR(shared_allocator) \
				Segregator<257, BUCKETIZER_FREELIST(8, 256, 8,shared_allocator), \
					Segregator<513, BUCKETIZER_FREELIST(257, 512, 16,shared_allocator), \
						Segregator<1025, BUCKETIZER_FREELIST(513, 1024, 32,shared_allocator), \
							FreeBlkList<StackAllocator<BlkListStackSize, Mallocator>, BlkListBlkSize, BlkListStackSize/BlkListBlkSize>>>> 

		Allocator* m_Allocator;
		BinarySearchTree<Blk, BlkComparator> m_Lookup;

		typedef BSTNode<Blk, BlkComparator> MemManBlkNode;
	};

}

extern void* ZMAlloc(size_t size);
extern void* ZMCalloc(size_t size, size_t val);
extern void  ZMFree(void* pointerToDelete);
extern void* ZMRealloc(void* data, size_t size);

extern MEMORY_DLL ZMasher::MemoryManager g_MemoryManager;
extern ZMasher::MemoryManager* g_Derp;