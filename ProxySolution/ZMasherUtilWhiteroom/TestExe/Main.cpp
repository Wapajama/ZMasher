#include "../ZMasherUtilWhiteroom/ZMVector.h"

using namespace ZMasher;

int main()
{
	Vector3f test(1,2,3);
	Vector3f test2(1,1,1);

	for (long long i = 0; i < 5000000000; ++i)
	{
		test += test2;
	}

	return 0;
}