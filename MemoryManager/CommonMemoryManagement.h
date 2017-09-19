#pragma once

#ifdef MEMORY_DLL_EXPORT
#define MEMORY_DLL __declspec(dllexport)
#else
#define MEMORY_DLL __declspec(dllimport)
#endif // MEMORY_DLL_EXPORT

#include <DataStructures\BinarySearchTree.h>
typedef int MemSizeType;
namespace ZMasher
{
	struct Blk
	{
		void* m_Data;
		MemSizeType m_Size;
	};

	class BlkComparator
		: public BSTComparator<Blk>
	{
	public:
		bool LessThan(const Blk& one,const Blk& two)const override
		{
			return reinterpret_cast<MemSizeType>(one.m_Data) < reinterpret_cast<MemSizeType>(two.m_Data);
		}
		bool GreaterThan(const Blk& one,const Blk& two)const override
		{
			return reinterpret_cast<MemSizeType>(one.m_Data) > reinterpret_cast<MemSizeType>(two.m_Data);
		}
		bool Equals(const Blk& one,const Blk& two)const override
		{
			return reinterpret_cast<MemSizeType>(one.m_Data) == reinterpret_cast<MemSizeType>(two.m_Data);
		}

	};
}
#define NULL_BLK {nullptr, 0}