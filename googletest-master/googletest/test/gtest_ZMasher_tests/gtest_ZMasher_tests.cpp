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

const MemSizeType BlkListStackSize = 1024*1024*16;
const MemSizeType BlkListBlkSize = 256;
typedef Segregator<257, Bucketizer<FreeList<Mallocator,-1, 256>, 1, 256, 8>, 
	FreeBlkList<StackAllocator<BlkListStackSize, Mallocator>,
	BlkListBlkSize, BlkListStackSize/BlkListBlkSize>>
	MainAllocatorType;

typedef StackAllocator<1024, Mallocator> AllocatorType;

static MainAllocatorType* g_allocator = nullptr;

GTEST_API_ int main(int argc, char **argv)
{
	printf("Running main() from gtest_ZMasher.tests.cpp\n");
	srand((0));
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
		delete m_Allocator;
		m_Allocator = new MainAllocatorType();
	}

	MainAllocatorType* m_Allocator = nullptr;

};

static Blk g_small_blk;
static Blk g_medium_blk;
static Blk g_large_blk;
static Blk g_very_large_blk;

//TEST_F(ZMasherAllocatorTest, Constructor)
//{
//}

constexpr MemSizeType loc_small_alloc_size = 8;
constexpr MemSizeType loc_medium_alloc_size = 256;
constexpr MemSizeType loc_large_alloc_size = 8192;
constexpr MemSizeType loc_very_large_alloc_size = 1024*1024;

//TEST_F(ZMasherAllocatorTest, SmallAllocate)
//{
//	g_small_blk = m_Allocator->Allocate(loc_small_alloc_size);
//
//	EXPECT_NE(g_small_blk.m_Data, nullptr);
//	EXPECT_EQ(g_small_blk.m_Size, loc_small_alloc_size);
//	EXPECT_TRUE(m_Allocator->Owns(g_small_blk));
//}
//
//TEST_F(ZMasherAllocatorTest, SmallDeallocate)
//{
//	(m_Allocator->Deallocate(g_small_blk));
//
//	EXPECT_FALSE(m_Allocator->Owns(g_small_blk));
//}
//
//TEST_F(ZMasherAllocatorTest, LargeAllocate)
//{
//	g_large_blk = m_Allocator->Allocate(loc_large_alloc_size);
//
//	EXPECT_NE(g_large_blk.m_Data, nullptr);
//	EXPECT_EQ(g_large_blk.m_Size , loc_large_alloc_size);
//	EXPECT_TRUE(m_Allocator->Owns(g_large_blk));
//}
//
//TEST_F(ZMasherAllocatorTest, LargeDeallocate)
//{
//	EXPECT_NO_FATAL_FAILURE(m_Allocator->Deallocate(g_large_blk));
//
//	EXPECT_FALSE(m_Allocator->Owns(g_large_blk));
//}
//
//TEST_F(ZMasherAllocatorTest, VeryLargeAllocate)
//{
//	g_very_large_blk = m_Allocator->Allocate(loc_very_large_alloc_size);
//
//	EXPECT_NE(g_very_large_blk.m_Data, nullptr);
//	EXPECT_EQ(g_very_large_blk.m_Size, loc_very_large_alloc_size);
//	EXPECT_TRUE(m_Allocator->Owns(g_very_large_blk));
//}
//
//TEST_F(ZMasherAllocatorTest, VeryLargeDeallocate)
//{
//	EXPECT_NO_FATAL_FAILURE(m_Allocator->Deallocate(g_very_large_blk));
//
//	EXPECT_FALSE(m_Allocator->Owns(g_very_large_blk));
//}

float GetRandomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int GetRandomInt(int min, int max)
{
	return (rand() % (max + 1 - min))+min;
}

const MemSizeType small_allocation_min_size = 8;
const MemSizeType small_allocation_max_size = 256;
//TEST_F(ZMasherAllocatorTest, MultipleSmallAllocate)
//{
//	const MemSizeType n_of_allocations = 128;
//	GrowArray<Blk> blks(n_of_allocations);
//	
//	for (MemSizeType i = 0; i < n_of_allocations; i++)
//	{
//		blks.Add(m_Allocator->Allocate(GetRandomInt(small_allocation_min_size, small_allocation_max_size)));
//	}
//	
//	int owns = 0;
//	int too_big = 0;
//	for (MemSizeType i = 0; i < n_of_allocations; i++)
//	{
//		if (m_Allocator->Owns(blks[i]))
//		{
//			++owns;
//		}
//		if (blks[i].m_Size > small_allocation_max_size)
//		{
//			++too_big;
//		}
//	}
//	EXPECT_EQ(owns, n_of_allocations);
//	//EXPECT_EQ(too_big, 0);
//
//	for (MemSizeType i = 0; i < n_of_allocations; i++)
//	{
//		m_Allocator->Deallocate(blks[i]);
//	}
//	m_Allocator->DeallocateAll();
//	owns = 0;
//	for (MemSizeType i = 0; i < n_of_allocations; i++)
//	{
//		if (m_Allocator->Owns(blks[i]))
//		{
//			++owns;
//		}
//	}
//	EXPECT_EQ(owns, 0);
//	blks.RemoveAll();
//	for (MemSizeType i = 0; i < n_of_allocations; i++)
//	{
//		blks.Add(m_Allocator->Allocate(GetRandomInt(small_allocation_min_size, small_allocation_max_size)));
//	}
//
//	m_Allocator->DeallocateAll();
//
//	owns = 0;
//	for (MemSizeType i = 0; i < n_of_allocations; i++)
//	{
//		if (m_Allocator->Owns(blks[i]))
//		{
//			++owns;
//		}
//	}
//	EXPECT_EQ(owns, 0);
//}

const MemSizeType stress_small_allocation_count = 1024*32;

TEST_F(ZMasherAllocatorTest, StressSmallAllocate)
{
	GrowArray<Blk, MemSizeType, stress_small_allocation_count> blks;

	for (MemSizeType i = 0; i < stress_small_allocation_count; i++)
	{
		blks.Add(m_Allocator->Allocate(GetRandomInt(8, small_allocation_max_size)));
	}

	for (MemSizeType i = 0; i < blks.Size(); i++)
	{
		m_Allocator->Deallocate(blks[i]);
	}

	blks.RemoveAll();

	for (MemSizeType i = 0; i < stress_small_allocation_count; i++)
	{
		blks.Add(m_Allocator->Allocate(GetRandomInt(8, small_allocation_max_size)));
	}

	int* temp_char = nullptr;
	for (MemSizeType i = 0; i < blks.Size(); i++)
	{
		temp_char = reinterpret_cast<int*>( blks[i].m_Data);
		(*temp_char) = blks[i].m_Size;
		temp_char = reinterpret_cast<int*>( blks[i].m_Data);
	}

	for (MemSizeType i = 0; i < blks.Size()/2; i++)
	{
		const int index = GetRandomInt(0, (stress_small_allocation_count-1)/2);
		m_Allocator->Deallocate(blks[index]);
		blks.RemoveCyclic(index);
	}

	int incorrect_value = 0;
	for (MemSizeType i = 0; i < blks.Size(); i++)
	{
		int first = blks[i].m_Size;
		int second = (*reinterpret_cast<int*>(blks[i].m_Data));
		if(first != second)
		{
			++incorrect_value;
		}
	}
	EXPECT_EQ(incorrect_value, 0);
}

const MemSizeType stress_large_allocation_count = 256;

const MemSizeType large_allocation_min_size = 256;
const MemSizeType large_allocation_max_size = 1024;

TEST_F(ZMasherAllocatorTest, StressLargeAllocate)
{
	GrowArray<Blk, MemSizeType, stress_large_allocation_count> blks;
	for (MemSizeType i = 0; i < stress_large_allocation_count; i++)
	{
		MemSizeType tmp = GetRandomInt(large_allocation_min_size, large_allocation_max_size);
		blks.Add(m_Allocator->Allocate(tmp));
		short* short_array = reinterpret_cast<short*>(blks.GetLast().m_Data);
		for (MemSizeType o = 0; o < tmp/2; o++)
		{
			short_array[o] = o;
		}
	}

	int incorrect_entries = 0;
	for (MemSizeType i = 0; i < blks.Size(); i++)
	{
		for (MemSizeType o = i + 1; o < blks.Size()-1; o++)
		{
			if (blks[i].m_Data == blks[o].m_Data)
			{
				++incorrect_entries;
			}
		}
	}
	EXPECT_EQ(incorrect_entries, 0);
	incorrect_entries = 0;
	//for (MemSizeType i = 0; i < blks.Size()/2; i++)
	//{
	//	const int index = GetRandomInt(0, (stress_large_allocation_count-1)/2);
	//	m_Allocator->Deallocate(blks[index]);
	//	blks.RemoveCyclic(index);
	//}
	for (MemSizeType i = 0; i < blks.Size(); i++)
	{
		short* short_array = reinterpret_cast<short*>(blks[i].m_Data);
		if (short_array[0] != 0)
		{
			++incorrect_entries;
		}
		for (MemSizeType o = 1; o < blks[i].m_Size/2; o++)
		{
			if (short_array[o-1] >= short_array[o])
			{
				++incorrect_entries;
			}
		}
	}
	EXPECT_EQ(incorrect_entries, 0);
	for (MemSizeType i = 0; blks.Size() >= stress_large_allocation_count; i++)
	{
		blks.Add(m_Allocator->Allocate(GetRandomInt(large_allocation_min_size, large_allocation_max_size)));
	}
	m_Allocator->DeallocateAll();
	blks.RemoveAll();


}