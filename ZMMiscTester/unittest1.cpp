#include "stdafx.h"
#include "CppUnitTest.h"
#include <DataStructures\GrowArray.h>
#include <assert.h>
#include <stdexcept>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZMMiscTester
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		inline void AddElements(GrowArray<int>& array, int n)
		{
			for (int i = 0; i < n; ++i)
			{
				array.Add(i);
			}
		}

		TEST_METHOD(GrowArrayCreate)
		{
			GrowArray<int> test;
		}

		TEST_METHOD(GrowArray_AddElements)
		{
			GrowArray<int> test;
			AddElements(test, 32);
			test[20] = 10;
			test[31] = 10;
			test[0] = 10;
		}

		TEST_METHOD(GrowArray_AddElementsShouldResize)
		{
			GrowArray<int> test;
			AddElements(test, 129);
			test[100] = 10;
			test[128] = 10;
			test[0] = 10;
		}

		TEST_METHOD(GrowArray_AddElementsShouldResize_ShouldFail)
		{
			auto crash = [this]()
			{
				GrowArray<int> test;
				AddElements(test, 129);
				test[129] = 100;
			};
			Assert::ExpectException<int>(crash);
		}

		TEST_METHOD(GrowArrayInit_ShouldFail)
		{
			auto crash = [this]()
			{
				GrowArray<int> test(10);
				test[1000] = 1;
			};
			Assert::ExpectException<int>(crash);
		}

		TEST_METHOD(GrowArray_ShouldNotResize)
		{
			GrowArray<int> test(32);
			Assert::IsTrue(test.MaxSize() == 32);
		}

		TEST_METHOD(GrowArray_ClearMisc)
		{
			GrowArray<int> test;
			AddElements(test, 32);
			test.RemoveCyclic(0);
			Assert::AreEqual(test[0], 31);
			Assert::IsTrue(test.Size() == 31);
			test.RemoveAll();
			Assert::IsTrue(test.Size() == 0);

			GrowArray<int*> test2;
			for (int i = 0; i < 32; ++i)
			{
				test2.Add(new int(i));
			}
			test2.DeleteCyclic(0);
			
			Assert::AreEqual(*test2[0], 31);
			Assert::IsTrue(test2.Size() == 31);
			test2.DeleteAll();
			Assert::IsTrue(test2.Size() == 0);

			//auto crash = [test2]()
			//{
			//	test2[0] = new int(10);
			//	return 0;
			//};
			//Assert::ExpectException<int>(crash);

			//GrowArray<int> ordered;
		}

		TEST_METHOD(GrowArray_RemoveAll_ShouldFail)
		{
			auto crash = [this]()
			{
				GrowArray<int> test;
				AddElements(test, 16);
				test.RemoveAll();
				test[10] = 0;
			};
			Assert::ExpectException<int>(crash);
		}

		TEST_METHOD(GrowArray_)
		{
			
		}
	};
}