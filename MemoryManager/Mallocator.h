#pragma once
#include <Allocator.h>
#include <DataStructures\BinarySearchTree.h>
#include <DataStructures\GrowArray.h>
#define MALLOCATOR_TEMPLATE //template<MemSizeType size, MemSizeType alignment = 1>
//#define MALLOCATOR_DECL //Mallocator<size, alignment>
#define MALLOCATOR_DECL Mallocator

typedef void* MallocPointer;

class MallocComparer
	: public ZMasher::BSTComparator<MallocPointer>
{
public:
	bool LessThan(const MallocPointer& one,const MallocPointer& two)const override
	{
		return one < two;
	}
	bool GreaterThan(const MallocPointer& one,const MallocPointer& two)const override
	{
		return one > two;
	}
	bool Equals(const MallocPointer& one,const MallocPointer& two)const override
	{
		return one == two;
	}
};

namespace ZMasher
{
	MALLOCATOR_TEMPLATE
	class MEMORY_DLL Mallocator :
		public Allocator
	{
	public:
		Mallocator();
		~Mallocator();

		BASE_ALLOCATOR_OVERRIDE_ALL;

	private:
		GrowArray<void*, MemSizeType, 1024> m_Lookup;

	};
}
