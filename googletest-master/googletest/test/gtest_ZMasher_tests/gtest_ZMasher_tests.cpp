// gtest_ZMasher_tests.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"
#include <StackAllocator.h>
#include <Mallocator.h>
#include <Segregator.h>
#include <FreeBlkList.h>
#include <Freelist.h>
#include <Bucketizer.h>

#include "../../../googletest/test/AllocatorTests.h"
using namespace ZMasher;

const MemSizeType BlkListStackSize = 1024*1024;
const MemSizeType BlkListBlkSize = 256;
typedef Segregator<256, Bucketizer<FreeList<Mallocator,-1, -1>, 1, 256, 8>, 
	FreeBlkList<StackAllocator<BlkListStackSize, Mallocator>,
	BlkListBlkSize, BlkListStackSize/BlkListBlkSize>>
	MainAllocatorType;

typedef StackAllocator<1024, Mallocator> AllocatorType;

static MainAllocatorType* g_allocator = nullptr;

GTEST_API_ int main(int argc, char **argv)
{
	printf("Running main() from gtest_main.cc\n");
	g_allocator = new MainAllocatorType();
	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	system("pause");
	return result;
}


class ZMasherAllocatorTest
	: public ::testing::Test
{
public:
	void SetUp()
	{
		m_Allocator = g_allocator;
	}

	MainAllocatorType* m_Allocator = nullptr;
	Blk g_small_blk;
	Blk g_medium_blk;
	Blk g_large_blk;
	Blk g_very_large_blk;
};

TEST_F(ZMasherAllocatorTest, Constructor)
{
	EXPECT_NO_FATAL_FAILURE(m_Allocator);
}

constexpr MemSizeType loc_small_alloc_size = 8;
constexpr MemSizeType loc_medium_alloc_size = 256;
constexpr MemSizeType loc_large_alloc_size = 8192;
constexpr MemSizeType loc_very_large_alloc_size = 1024*1024;



TEST_F(ZMasherAllocatorTest, SmallAllocate)
{
	g_small_blk = m_Allocator->Allocate(loc_small_alloc_size);

	EXPECT_TRUE(g_small_blk.m_Data != nullptr);
	EXPECT_TRUE(g_small_blk.m_Size == loc_small_alloc_size);
	EXPECT_TRUE(m_Allocator->Owns(g_small_blk));
}

TEST_F(ZMasherAllocatorTest, SmallDeallocate)
{
	EXPECT_NO_FATAL_FAILURE(m_Allocator->Deallocate(g_small_blk));
}

