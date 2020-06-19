#pragma once
#include "Allocator.h"
#include <DataStructures\GrowArray.h>

#define FREE_BLK_LIST_TEMPL template<typename BaseAllocator, MemSizeType blk_size, MemSizeType nr_of_blks>
#define FREE_BLK_LIST_DECL FreeBlkList<BaseAllocator, blk_size, nr_of_blks>

namespace ZMasher
{

	FREE_BLK_LIST_TEMPL
	class FreeBlkList
		: private BaseAllocator
	{
	public:
		FreeBlkList();
		~FreeBlkList();

		BASE_ALLOCATOR_ALL_METHODS

	private:

		const MemSizeType m_Size = blk_size * nr_of_blks;

		Blk m_Data;
		//specifies all vacant memoryblocks. All memoryblocks are blk_size big
		//true if vacant, false if occupied
		GrowArray<bool, MemSizeType, nr_of_blks, 1> m_Blks;

		inline MemSizeType GetIndex(void* data)
		{
			 return ((MEMSIZETYPE_CAST(data) - MEMSIZETYPE_CAST(m_Data.m_Data)) / blk_size);
		}
		MemSizeType m_AllocatedSize;
	};

	FREE_BLK_LIST_TEMPL
	FREE_BLK_LIST_DECL::FreeBlkList()
	{
		m_AllocatedSize = 0;
		m_Data = BaseAllocator::Allocate(m_Size);
		for (MemSizeType i = 0; i < nr_of_blks; ++i)
		{
			m_Blks.Add(true);
		}
	}
	FREE_BLK_LIST_TEMPL
	FREE_BLK_LIST_DECL::~FreeBlkList()
	{
		//BaseAllocator::Deallocate(m_Data);
	}

	FREE_BLK_LIST_TEMPL
	bool FREE_BLK_LIST_DECL::GoodSize(MemSizeType size)
	{
		//find so many consecutive blocks so that it will
		//have room for the size, remove those indeces and
		//return pointer from the first block
		MemSizeType indeces_needed = (size/blk_size);
		((indeces_needed * blk_size) == size) ? indeces_needed: ++indeces_needed;

		MemSizeType blk_index = 0;
		MemSizeType counter = 0;
		for (MemSizeType i = 0; i < m_Blks.Size(); ++i)
		{
			if (m_Blks[i])
			{
				if (counter == 0)
				{
					blk_index = i;
				}
				++counter;
			}
			else
			{
				counter = 0;
			}
			if (counter == indeces_needed)
			{
				return true;
			}
		}
		return false;
	}
	FREE_BLK_LIST_TEMPL
	Blk FREE_BLK_LIST_DECL::Allocate(MemSizeType size)
	{
		if (!this->FREE_BLK_LIST_DECL::GoodSize(size))
		{
			return BaseAllocator::Allocate(size);
		}

		MemSizeType indeces_needed = (size/blk_size);
		((indeces_needed * blk_size) == size) ? 
			indeces_needed: 
			++indeces_needed;

		m_AllocatedSize += size;

		MemSizeType blk_index = 0;
		MemSizeType counter = 0;

		for (MemSizeType i = 0; i < m_Blks.Size(); ++i)
		{
			if (m_Blks[i])
			{
				if (counter == 0)
				{
					blk_index = i;
				}
				++counter;
			}
			else
			{
				counter = 0;
			}
			if (counter == indeces_needed)
			{
				for (MemSizeType o = blk_index; o < (blk_index + indeces_needed); ++o)
				{
					ASSERT(m_Blks[o], "Can't allocate already allocated space!");
					m_Blks[o] = false; //allocating space
				}
				return { (TO_DATA_PTR( MEMSIZETYPE_CAST(m_Data.m_Data) + blk_size*blk_index)), size};
			}
		}
		return NULL_BLK;
	}
	FREE_BLK_LIST_TEMPL
	Blk FREE_BLK_LIST_DECL::AllocateAll()
	{
		// TODO: implement this
		return NULL_BLK;
	}
	FREE_BLK_LIST_TEMPL
	Blk FREE_BLK_LIST_DECL::AllocateAligned(MemSizeType size, MemSizeType alignment)
	{
		// TODO: implement this
		return NULL_BLK;
	}
	FREE_BLK_LIST_TEMPL
	Blk FREE_BLK_LIST_DECL::AllocateAllAligned(MemSizeType alignment)
	{
		// TODO: implement this
		return NULL_BLK;
	}
	FREE_BLK_LIST_TEMPL
	bool FREE_BLK_LIST_DECL::Expand(Blk& blk, MemSizeType delta)
	{
		MemSizeType data_derp = this->GetIndex(blk.m_Data);
		MemSizeType index = this->GetIndex( TO_DATA_PTR( MEMSIZETYPE_CAST(blk.m_Data) + blk.m_Size));

		for (MemSizeType i = index; delta - blk_size*(i-index) > 0 ; i++)
		{
			if (i >= m_Blks.Size() ||
				!m_Blks[i])
			{
				return false;
			}
		}
		for (MemSizeType i = index; delta - blk_size*(i-index) > 0; i++)
		{
			ASSERT(m_Blks[i], "Can't allocate allocated space!");
			m_Blks[i] = false;
		}
		blk.m_Size += delta;

		return true;
	}
	FREE_BLK_LIST_TEMPL
	void FREE_BLK_LIST_DECL::Reallocate(Blk& blk, MemSizeType size)
	{
		if (!this->FREE_BLK_LIST_DECL::Owns(blk))
		{
			if (BaseAllocator::Owns(blk))
			{
				BaseAllocator::Reallocate(blk, size);
			}
			return;
		}
		if (blk.m_Size == size)
		{
			return;
		}
		//smaller
		if (size < blk.m_Size)
		{
			//deallocate the blocks at the end, if any
			const MemSizeType diff = blk.m_Size - size;
			const MemSizeType blks_to_dealloc = diff/blk_size;
			MemSizeType padding = 0;
			if (size % blk_size != 0)
			{
				++padding;
			}
			for (MemSizeType i = 0; i < blks_to_dealloc; ++i)
			{
				const MemSizeType index = GetIndex(TO_DATA_PTR(MEMSIZETYPE_CAST( blk.m_Data ) + blk_size * i + size) ) + padding;
				ASSERT(m_Blks[index] == false, "Can't deallocate free memory!");
				m_Blks[index] = true;
			}
			m_AllocatedSize -= (blk.m_Size - size);
			blk.m_Size = size;
		}
		else
		{
			m_AllocatedSize += (size - blk.m_Size);
			if (!this->FREE_BLK_LIST_DECL::Expand(blk, size - blk.m_Size))
			{
				Blk tmp_blk = this->FREE_BLK_LIST_DECL::Allocate(size);

				ASSERT(tmp_blk.m_Data, "FreeBlkList, failed to realloc!");
				if (tmp_blk.m_Data != nullptr)
				{
#ifdef DEBUG
					OutputDebugStringA("\nFreeBlkList reallocate memcpy");
#endif // DEBUG

					memcpy(tmp_blk.m_Data, blk.m_Data, min(blk.m_Size, size));

					this->FREE_BLK_LIST_DECL::Deallocate(blk);
				}
				blk = tmp_blk;
			}
		}
	}
	FREE_BLK_LIST_TEMPL
	bool FREE_BLK_LIST_DECL::Owns(Blk blk)
	{
		if(	MEMSIZETYPE_CAST(blk.m_Data) <  MEMSIZETYPE_CAST(m_Data.m_Data) + blk_size * nr_of_blks &&
			MEMSIZETYPE_CAST(blk.m_Data) >= MEMSIZETYPE_CAST(m_Data.m_Data))
		{
			if (!m_Blks[GetIndex(blk.m_Data)])
			{
				return true;
			}
		}
		return false;
	}
	FREE_BLK_LIST_TEMPL
	void FREE_BLK_LIST_DECL::Deallocate(Blk blk)
	{
		if (!this->FREE_BLK_LIST_DECL::Owns(blk))
		{
			if (BaseAllocator::Owns(blk))
			{
				BaseAllocator::Deallocate(blk);
			}
			else
			{
				ASSERT(false, "Blk isn't allocated by this allocator");
			}
			return;
		}

		//find what block it is in the list
		//set the blocks to true until we have reached its size
		const MemSizeType index = GetIndex(blk.m_Data);
		MemSizeType padding = 0;
		if (blk.m_Size % blk_size != 0)
		{
			++padding;
		}
		for (MemSizeType i = 0; i < (blk.m_Size/blk_size) + padding; ++i)
		{
			ASSERT(m_Blks[i+index] == false, "FreeBlkList: Can't deallocate free memory!");
			m_Blks[i + index] = true;
		}
	}
	FREE_BLK_LIST_TEMPL
	void FREE_BLK_LIST_DECL::DeallocateAligned(Blk blk)
	{
		//TODO: Implement this
	}
	FREE_BLK_LIST_TEMPL
	void FREE_BLK_LIST_DECL::DeallocateAll()
	{
		for (MemSizeType i = 0; i < m_Blks.Size(); i++)
		{
			m_Blks[i] = true;
		}
	}
}