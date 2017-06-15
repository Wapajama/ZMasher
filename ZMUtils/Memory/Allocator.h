#pragma once
#include <Memory/CommonMemoryManagement.h>

//base class for all allocators
namespace ZMasher
{
	class Allocator
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

#define BASE_ALLOCATOR_OVERRIDE_ALL bool GoodSize(MemSizeType size)override;\
		Blk Allocate(MemSizeType size)override;\
		Blk AllocateAll()override;\
		Blk AllocateAligned(MemSizeType size, MemSizeType alignment)override;\
		Blk AllocateAllAligned(MemSizeType alignment)override;\
		bool Expand(Blk& blk, MemSizeType delta)override;\
		void Reallocate(Blk& blk, MemSizeType size)override;\
		bool Owns(Blk blk)override;\
		void Deallocate(Blk blk)override;\
		void DeallocateAligned(Blk blk)override;\
		void DeallocateAll()override;

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
//Blk Allocator::AllocateAligned(MemSizeType size, unsigned alignment)
//{
//}
//Blk Allocator::AllocateAllAligned(unsigned alignment)
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
//void Allocator::DeallocateAll()
//{
//}