//#include "../ZMasherUtilWhiteroom/ZMVector.h"
//#include "../ZMasherUtilWhiteroom/ZMMatrix44.h"

#include <ZMMatrix44.h>

using namespace ZMasher;

int main()
{
	Matrix44f tester1 = Matrix44f(	1 , 2 , 3 , 4,
									5 , 6 , 7 , 8,
									9 , 10, 11, 12,
									13, 14, 15, 16);

	Matrix44f tester2 = Matrix44f(12, 23, 34, 45,
								  56, 67, 78, 89,
								  13, 24, 35, 46,
								  57, 68, 79, 14);

	tester1 = tester1 * tester2;

	return 0;
}