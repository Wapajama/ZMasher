#include <ZMUtils/Math/ZMVector3.h>
#include <ZMUtils/Math/ZMVector4.h>

namespace ZMasher
{
	Vector3f::Vector3f(const Vector4f& vector4)
	{
		x = vector4.x;
		y = vector4.y;
		z = vector4.z;
	}
	Vector3f::Vector3f()
	{
		x = CAST(0);
		y = CAST(0);
		z = CAST(0);
	}
	Vector3f::Vector3f(const Vector3f& copy)
	{
		this->x = copy.x;
		this->y = copy.y;
		this->z = copy.z;
	}
	Vector3f::Vector3f(const float ax, const float ay, const float az)
	{
		x = ax;
		y = ay;
		z = az;
	}
	Vector3f::Vector3f(float* arrayPtr)
	{
		assert(arrayPtr != nullptr && "Vector3f: Constructorargument is null!");
		x = arrayPtr[0];
		y = arrayPtr[1];
		z = arrayPtr[2];
	}
	Vector3f::~Vector3f()
	{
	}
}