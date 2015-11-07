#ifndef VECTOR3_H
#define VECTOR3_H
#include <assert.h>

namespace ZMasher
{
	template<typename T>
	float Length(const Vector3<T>& aVector)
	{
		return sqrt((aVector.x * aVector.x) + (aVector.y * aVector.y) + (aVector.z * aVector.z));
	}

	template<typename T>
	float Length2(const Vector3<T>& aVector)
	{
		return (aVector.x * aVector.x) + (aVector.y * aVector.y) + (aVector.z * aVector.z);
	}

	template<typename T>
	void Normalize(Vector3<T>& aVector)
	{
		if (Length(aVector) > 0)
		{
			aVector /= Length(aVector);
		}
	}

	template<typename T>
	void MoveTowards(Vector3<T>& aPos, Vector3<T>& aGoalPos, float aSpeedWithDelta)
	{
		Vector3f dir = aGoalPos - aPos;
		Normalize(dir);
		aPos = aPos + dir * aSpeedWithDelta;
	}

	template<typename T>//made for floating point precision
	bool ApproxEqual(const Vector3<T>& aFirst, const Vector3<T>& aSecond, T aMargin = 0.0001)
	{
		return (aFirst.x + aMargin > aSecond.x && 
				aFirst.x - aMargin < aSecond.x &&

				aFirst.y + aMargin > aSecond.y && 
				aFirst.y - aMargin < aSecond.y &&

				aFirst.z + aMargin > aSecond.z &&
				aFirst.z - aMargin < aSecond.z);// == false;
	}

	template<typename T>
	Vector3<T> Lerp(Vector3<T> &aFirstPoint, Vector3<T> &aSecondPoint, float aAlpha)
	{
		return aFirstPoint + ((aSecondPoint - aFirstPoint) * aAlpha);
	}

	template<typename T>
	Vector3<T> GetNormalized(const Vector3<T>& aVector)
	{
		Vector3<T> returnVal(aVector);

		returnVal /= Length(aVector);
		return returnVal;
	}

	template<typename T>
	T Dot(const Vector3<T>& aVector, const Vector3<T>& aSecondVector)
	{
		return ((aVector.x * aSecondVector.x) + (aVector.y * aSecondVector.y) + (aVector.z * aSecondVector.z));
	}

	template<typename T>
	Vector3<T> Cross(const Vector3<T>& aVector, const Vector3<T>& aSecondVector)
	{
		Vector3<T> returnVal;

		returnVal.x = (aVector.y * aSecondVector.z) - (aVector.z * aSecondVector.y);
		returnVal.y = (aVector.z * aSecondVector.x) - (aVector.x * aSecondVector.z);
		returnVal.z = (aVector.x * aSecondVector.y) - (aVector.y * aSecondVector.x);

		return (returnVal);
	}

	template<typename T>
	Vector3<T> operator+(const Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		Vector3<T> returnVal(aLeftVector);

		returnVal.x += aRightVector.x;
		returnVal.y += aRightVector.y;
		returnVal.z += aRightVector.z;

		return (returnVal);
	}

	template<typename T>
	Vector3<T> operator-(const Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		Vector3<T> returnVal(aLeftVector);

		returnVal.x -= aRightVector.x;
		returnVal.y -= aRightVector.y;
		returnVal.z -= aRightVector.z;

		return (returnVal);
	}

	template<typename T>
	Vector3<T> operator*(const Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		Vector3<T> returnVal(aLeftVector);

		returnVal.x *= aRightVector.x;
		returnVal.y *= aRightVector.y;
		returnVal.z *= aRightVector.z;

		return (returnVal);
	}

	template<typename T>
	Vector3<T> operator/(const Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		Vector3<T> returnVal(aLeftVector);

		returnVal.x /= aRightVector.x;
		returnVal.y /= aRightVector.y;
		returnVal.z /= aRightVector.z;

		return (returnVal);
	}

	template<typename T>
	Vector3<T>& operator+=(Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		aLeftVector.x += aRightVector.x;
		aLeftVector.y += aRightVector.y;
		aLeftVector.z += aRightVector.z;

		return (aLeftVector);
	}

	template<typename T>
	Vector3<T>& operator-=(Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		aLeftVector.x -= aRightVector.x;
		aLeftVector.y -= aRightVector.y;
		aLeftVector.z -= aRightVector.z;

		return (aLeftVector);
	}

	template<typename T>
	Vector3<T>& operator*=(Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		aLeftVector.x *= aRightVector.x;
		aLeftVector.y *= aRightVector.y;
		aLeftVector.z *= aRightVector.z;

		return (aLeftVector);
	}

	template<typename T>
	Vector3<T>& operator/=(Vector3<T>& aLeftVector, const Vector3<T>& aRightVector)
	{
		aLeftVector.x /= aRightVector.x;
		aLeftVector.y /= aRightVector.y;
		aLeftVector.z /= aRightVector.z;

		return (aLeftVector);
	}

	template<typename T>
	Vector3<T> operator*(const Vector3<T>& aLeftVector, const float aScalar)
	{
		Vector3<T> returnVal(aLeftVector);

		returnVal.x *= aScalar;
		returnVal.y *= aScalar;
		returnVal.z *= aScalar;

		return (returnVal);
	}

	template<typename T>
	Vector3<T> operator/(const Vector3<T>& aLeftVector, const float aScalar)
	{
		Vector3<T> returnVal(aLeftVector);

		returnVal.x /= aScalar;
		returnVal.y /= aScalar;
		returnVal.z /= aScalar;

		return (returnVal);
	}

	template<typename T>
	Vector3<T>& operator/=(Vector3<T>& aLeftVector, const float aScalar)
	{
		aLeftVector.x /= aScalar;
		aLeftVector.y /= aScalar;
		aLeftVector.z /= aScalar;

		return (aLeftVector);
	}

	template<typename T>
	Vector3<T>& operator*=(Vector3<T>& aLeftVector, const float aScalar)
	{
		aLeftVector.x *= aScalar;
		aLeftVector.y *= aScalar;
		aLeftVector.z *= aScalar;

		return (aLeftVector);
	}

	template<typename T>
	bool operator!=(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return ((aLeft.x != aRight.x) || (aLeft.y != aRight.y) || (aLeft.z != aRight.z));
	}

	template<typename T>
	bool operator==(const Vector3<T>& aLeft, const Vector3<T>& aRight)
	{
		return ((aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z));
	}

	template<typename T>
	T Vector3<T>::operator[](const short aIndex) const
	{
		assert(aIndex >= 0 && aIndex <= 2 && "Vector::operator[] OUT OF RANGE");

		if (aIndex == 0)
			return x;
		else if (aIndex == 1)
			return y;
		else
			return z;

		return 0;
	}

	template<typename T>
	T& Vector3<T>::operator[](const short aIndex)
	{
		assert(aIndex >= 0 && aIndex <= 2 && "Vector::operator[] OUT OF RANGE");

		if (aIndex == 0)
			return x;
		else if (aIndex == 1)
			return y;


		return z;
	}
};

#endif
