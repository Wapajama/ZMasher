#pragma once
#include "Allocator.h"
#define FREE_BLK_LIST_TEMPL template<int blk_size, int no_blks>
#define FREE_BLK_LIST_DECL FreeBlkList<blk_size, no_blks>

FREE_BLK_LIST_TEMPL
class FreeBlkList
{
public:
	FreeBlkList();
	~FreeBlkList();

	BASE_ALLOCATOR_ALL_METHODS

private:

};

FREE_BLK_LIST_TEMPL
FREE_BLK_LIST_DECL::FreeBlkList()
{
}

FREE_BLK_LIST_TEMPL
FREE_BLK_LIST_DECL::~FreeBlkList()
{
}