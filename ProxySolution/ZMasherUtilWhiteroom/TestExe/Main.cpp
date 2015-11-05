#include "../ZMasherUtilWhiteroom/ZMVector.h"

using namespace ZMasher;

int main()
{
 	const long long largeNumber = 10000;

	//Vector3f test(1,2,3);
	//Vector3f test2(1,1,1);

	Vector3f testArray[largeNumber];
	Vector3f testArray2[largeNumber];

	for (int i = 0; i < largeNumber; i++)
	{
		testArray[i] = Vector3f(1.f, 2.f, 3.f);
		testArray2[i] = Vector3f(1.f, 1.f, 1.f);
	}

	for (long long i = 0; i < 5000000000; ++i)
	{
		//testArray[i%largeNumber] = testArray[i%largeNumber] + testArray2[(i - 1)%largeNumber];
		testArray[i%largeNumber] += testArray2[(i)%largeNumber];
	}

	return 0;
}