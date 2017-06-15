#pragma once

typedef int MemSizeType;
namespace ZMasher
{
	struct Blk
	{
		void* m_Data;
		MemSizeType m_Size;
	};

}
#define NULL_BLK {nullptr, 0}