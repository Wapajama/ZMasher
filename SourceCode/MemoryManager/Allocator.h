#pragma once
#include <CommonMemoryManagement.h>
#include <Windows.h>
//base class for all allocators
namespace ZMasher
{
	class MEMORY_DLL Allocator
	{
	public:
		Allocator(MemSizeType alignment = 1);
		virtual ~Allocator();

		const MemSizeType alignment;
		virtual bool GoodSize(MemSizeType size)=0;
		virtual Blk Allocate(MemSizeType size)=0;
		virtual Blk AllocateAll()=0;
		virtual Blk AllocateAligned(MemSizeType size, MemSizeType alignment)=0;
		virtual Blk AllocateAllAligned(MemSizeType alignment)=0;
		virtual bool Expand(Blk& blk, MemSizeType delta){return false;};
		virtual void Reallocate(Blk& blk, MemSizeType size)=0;
		virtual bool Owns(Blk blk)=0;
		virtual void Deallocate(Blk blk)=0;
		virtual void DeallocateAligned(Blk blk)=0;
		virtual void DeallocateAll()=0;

	};
}
#define BASE_ALLOCATOR_ALL_METHODS bool GoodSize(MemSizeType size);\
		virtual Blk Allocate(MemSizeType size);\
		virtual Blk AllocateAll();\
		virtual Blk AllocateAligned(MemSizeType size, MemSizeType alignment);\
		virtual Blk AllocateAllAligned(MemSizeType alignment);\
		virtual bool Expand(Blk& blk, MemSizeType delta);\
		virtual void Reallocate(Blk& blk, MemSizeType size);\
		virtual bool Owns(Blk blk);\
		virtual void Deallocate(Blk blk);\
		virtual void DeallocateAligned(Blk blk);\
		virtual void DeallocateAll();

#define BASE_ALLOCATOR_OVERRIDE_ALL bool GoodSize(MemSizeType size)override;\
		virtual Blk Allocate(MemSizeType size)override;\
		virtual Blk AllocateAll()override;\
		virtual Blk AllocateAligned(MemSizeType size, MemSizeType alignment)override;\
		virtual Blk AllocateAllAligned(MemSizeType alignment)override;\
		virtual bool Expand(Blk& blk, MemSizeType delta)override;\
		virtual void Reallocate(Blk& blk, MemSizeType size)override;\
		virtual bool Owns(Blk blk)override;\
		virtual void Deallocate(Blk blk)override;\
		virtual void DeallocateAligned(Blk blk)override;\
		virtual void DeallocateAll()override;

// ------- COPY PASTABLE FUNCTIONS ------
//bool Allocator::GoodSize(MemSizeType size)
//{
//}
//Blk Allocator::Allocate(MemSizeType size)
//{
//}
//Blk Allocator::AllocateAll()
//{
//}
//Blk Allocator::AllocateAligned(MemSizeType size, MemSizeType alignment)
//{
//}
//Blk Allocator::AllocateAllAligned(MemSizeType alignment)
//{
//}
//bool Allocator::Expand(Blk& blk, MemSizeType delta)
//{
//}
//void Allocator::Reallocate(Blk& blk, MemSizeType size)
//{
//}
//bool Allocator::Owns(Blk blk)
//{
//}
//void Allocator::Deallocate(Blk blk)
//{
//}
//void Allocator::DeallocateAligned(Blk blk)
//{
//}
//void Allocator::DeallocateAll()
//{
//}