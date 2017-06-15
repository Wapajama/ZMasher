#include "DynamicAllocation.h"
#include "MemoryManager.h"

void* operator new (size_t size)
{
	return ZMasher::MemoryManager::Instance()->Allocate(size);
}

void* operator new[](size_t size)
{
	return ZMasher::MemoryManager::Instance()->Allocate(size);
}

void operator delete (void* pointerToDelete)
{
	ZMasher::MemoryManager::Instance()->Free(pointerToDelete);
}

void operator delete[](void* arrayToDelete)
{
	ZMasher::MemoryManager::Instance()->Free(arrayToDelete);
}