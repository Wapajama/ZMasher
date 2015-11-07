#include "ZMasherUtilities.h"

#define _USE_MATH_DEFINES

#include <math.h>

namespace ZMasher
{

	float GetRadians(float degrees)
	{
		return degrees * (M_PI / 180.f);
	}

	float GetDegrees(float radians)
	{
		return radians / (M_PI/180.f);
	}

}