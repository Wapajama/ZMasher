//#include "../ZMasherUtilWhiteroom/ZMVector.h"
//#include "../ZMasherUtilWhiteroom/ZMMatrix44.h"

#include <ZMMatrix44.h>
#include <StackArray.h>
#include <MemoryContainer.h>


using namespace ZMasher;

int main()
{
	StackArray<int*, 1024, int> s_array;
	MemoryContainer<int, 1024, int> test;


	for (int i = 0; i < 1024; ++i)
	{
		int* test2 = test.GetData();
		(*test2) = i;
		s_array.Add(test2);
	}

	for (int i = 0; i < 1024; i++)
	{
		test.Delete(s_array[i]);
	}

	return 0;
}