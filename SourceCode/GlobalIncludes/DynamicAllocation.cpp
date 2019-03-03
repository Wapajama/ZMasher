#include <MemoryManager.h>
#include <new>
#include "project_defines.h"

#ifdef NEW_OP_OVERRIDE

ZMasher::MemoryManager* g_Derp = nullptr;

void* operator new (size_t size)throw(std::bad_alloc)
{
	return g_MemoryManager.Allocate(size);
}
void* operator new[](size_t size)throw(std::bad_alloc)
{
	return g_MemoryManager.Allocate(size);
}
void operator delete (void* pointerToDelete)throw()
{
	g_MemoryManager.Free(pointerToDelete);
}
void operator delete[](void* arrayToDelete)throw()
{
	g_MemoryManager.Free(arrayToDelete);
}

void* ZMAlloc(size_t size)throw(std::bad_alloc)
{
	return g_MemoryManager.Allocate(size);
}
void* ZMCalloc(size_t size, size_t val)
{
	void* data = ZMAlloc(size);
	return memset(data, val, size);
}
void ZMFree(void* pointerToDelete)throw()
{
	g_MemoryManager.Free(pointerToDelete);
}
void* ZMRealloc(void* data, size_t size)
{
	return g_MemoryManager.Reallocate(data, size);
}

//void* __declspec(selectany) operator new(size_t s, CDataBlockStore &pAllocator);
//{
//    D3DXASSERT( s <= 0xffffffff );
//    return pAllocator.Allocate( (UINT)s );
//}
//
//void __declspec(selectany) operator delete(void* p, CDataBlockStore &pAllocator);
//{
//}
#endif // NEW_OP_OVERRIDE
