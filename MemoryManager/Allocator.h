#pragma once
#include <CommonMemoryManagement.h>

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
#define BASE_ALLOCATOR_ALL_METHODS bool GoodSize(MemSizeType size);\
		Blk Allocate(MemSizeType size);\
		Blk AllocateAll();\
		Blk AllocateAligned(MemSizeType size, MemSizeType alignment);\
		Blk AllocateAllAligned(MemSizeType alignment);\
		bool Expand(Blk& blk, MemSizeType delta);\
		void Reallocate(Blk& blk, MemSizeType size);\
		bool Owns(Blk blk);\
		void Deallocate(Blk blk);\
		void DeallocateAligned(Blk blk);\
		void DeallocateAll();

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