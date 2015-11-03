#ifndef VECTOR4_H
#define VECTOR4_H

namespace ZMasher
{
	template<typename T>
	float Length(const Vector4<T>& aVector)
	{
		return sqrt((aVector.x * aVector.x) + (aVector.y * aVector.y) + (aVector.z * aVector.z) + (aVector.w * aVector.w));
	}

	template<typename T>
	float Length2(const Vector4<T>& aVector)
	{
		return (aVector.x * aVector.x) + (aVector.y * aVector.y) + (aVector.z * aVector.z) + (aVector.w * aVector.w);
	}

	template<typename T>
	void Normalize(Vector4<T>& aVector)
	{
		aVector /= Length(aVector);
	}

	template<typename T>
	Vector4<T> Lerp(Vector4<T> &aFirstPoint, Vector4<T> &aSecondPoint, float aAlpha)
	{
		return aFirstPoint + ((aSecondPoint - aFirstPoint) * aAlpha);
	}

	template<typename T>
	Vector4<T> GetNormalized(const Vector4<T>& aVector)
	{
		Vector4<T> returnVal(aVector);

		returnVal /= Length(aVector);
		return returnVal;
	}

	template<typename T>
	T Dot(const Vector4<T>& aVector, const Vector4<T>& aSecondVector)
	{
		return ((aVector.x * aSecondVector.x) + (aVector.y * aSecondVector.y) + (aVector.z * aSecondVector.z) + (aVector.w * aSecondVector.w));
	}

	template<typename T>
	Vector4<T> operator+(const Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		Vector4<T> returnVal(aLeftVector);

		returnVal.x += aRightVector.x;
		returnVal.y += aRightVector.y;
		returnVal.z += aRightVector.z;
		returnVal.w += aRightVector.w;

		return (returnVal);
	}

	template<typename T>
	Vector4<T> operator-(const Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		Vector4<T> returnVal(aLeftVector);

		returnVal.x -= aRightVector.x;
		returnVal.y -= aRightVector.y;
		returnVal.z -= aRightVector.z;
		returnVal.w -= aRightVector.w;

		return (returnVal);
	}

	template<typename T>
	Vector4<T> operator*(const Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		Vector4<T> returnVal(aLeftVector);

		returnVal.x *= aRightVector.x;
		returnVal.y *= aRightVector.y;
		returnVal.z *= aRightVector.z;
		returnVal.w *= aRightVector.w;

		return (returnVal);
	}

	template<typename T>
	Vector4<T> operator/(const Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		Vector4<T> returnVal(aLeftVector);

		returnVal.x /= aRightVector.x;
		returnVal.y /= aRightVector.y;
		returnVal.z /= aRightVector.z;
		returnVal.w /= aRightVector.w;

		return (returnVal);
	}

	template<typename T>
	Vector4<T>& operator+=(Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		aLeftVector.x += aRightVector.x;
		aLeftVector.y += aRightVector.y;
		aLeftVector.z += aRightVector.z;
		aLeftVector.w += aRightVector.w;

		return (aLeftVector);
	}

	template<typename T>
	Vector4<T>& operator-=(Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		aLeftVector.x -= aRightVector.x;
		aLeftVector.y -= aRightVector.y;
		aLeftVector.z -= aRightVector.z;
		aLeftVector.w -= aRightVector.w;

		return (aLeftVector);
	}

	template<typename T>
	Vector4<T>& operator*=(Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		aLeftVector.x *= aRightVector.x;
		aLeftVector.y *= aRightVector.y;
		aLeftVector.z *= aRightVector.z;
		aLeftVector.w *= aRightVector.w;

		return (aLeftVector);
	}

	template<typename T>
	Vector4<T>& operator/=(Vector4<T>& aLeftVector, const Vector4<T>& aRightVector)
	{
		aLeftVector.x /= aRightVector.x;
		aLeftVector.y /= aRightVector.y;
		aLeftVector.z /= aRightVector.z;
		aLeftVector.w /= aRightVector.w;

		return (aLeftVector);
	}

	template<typename T>
	Vector4<T> operator*(const Vector4<T>& aLeftVector, const float aScalar)
	{
		Vector4<T> returnVal(aLeftVector);

		returnVal.x *= aScalar;
		returnVal.y *= aScalar;
		returnVal.z *= aScalar;
		returnVal.w *= aScalar;

		return (returnVal);
	}

	template<typename T>
	Vector4<T> operator/(const Vector4<T>& aLeftVector, const float aScalar)
	{
		Vector4<T> returnVal(aLeftVector);

		returnVal.x /= aScalar;
		returnVal.y /= aScalar;
		returnVal.z /= aScalar;
		returnVal.w /= aScalar;

		return (returnVal);
	}

	template<typename T>
	Vector4<T>& operator/=(Vector4<T>& aLeftVector, const float aScalar)
	{
		aLeftVector.x /= aScalar;
		aLeftVector.y /= aScalar;
		aLeftVector.z /= aScalar;
		aLeftVector.w /= aScalar;

		return (aLeftVector);
	}

	template<typename T>
	Vector4<T>& operator*=(Vector4<T>& aLeftVector, const float aScalar)
	{
		aLeftVector.x *= aScalar;
		aLeftVector.y *= aScalar;
		aLeftVector.z *= aScalar;
		aLeftVector.w *= aScalar;

		return (aLeftVector);
	}

	template<typename T>
	void Vector4<T>::operator=(const Vector3<T>& aRightVector)
	{
		myX = aRightVector.x;
		myY = aRightVector.y;
		myZ = aRightVector.z;
		myW = NULL;
	}

};

#endif