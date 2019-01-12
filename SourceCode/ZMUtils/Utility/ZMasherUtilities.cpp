#include "ZMasherUtilities.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>

namespace ZMasher
{

	float GetRadians(float degrees)
	{
		return degrees * (M_PI / 180.f);
	}

	float GetDegrees(float radians)
	{
		return radians / (M_PI / 180.f);
	}

	float GetRandomFloat(float min, float max)
	{
		return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}

	int GetRandomInt(int min, int max)
	{
		return (rand() % (max + 1 - min))+min;
	}
}