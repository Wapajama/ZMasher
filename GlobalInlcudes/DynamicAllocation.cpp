//#include "DynamicAllocation.h"
#include <Memory\MemoryManager.h>
#include <new>

//TODO: Make this only create once, without having several instances of globals
//(Right now it creates three times, once per active project it seems)
ZMasher::MemoryManager g_MemoryManager;

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