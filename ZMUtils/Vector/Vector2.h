#ifndef VECTOR2_H
#define VECTOR2_H

namespace CommonUtilities
{
	template<typename T>
	float Length(const Vector2<T>& aVector)
	{
		return sqrt((aVector.x * aVector.x) + (aVector.y * aVector.y));
	}

	template<typename T>
	float Length2(const Vector2<T>& aVector)
	{
		return (aVector.x * aVector.x) + (aVector.y * aVector.y);
	}

	template<typename T>
	void Normalize(Vector2<T>& aVector)
	{
		aVector /= Length(aVector);
	}

	template<typename T>
	void MoveTowards(Vector2<T>& aPos, Vector2<T>& aGoalPos,float aSpeedWithDelta)
	{
		Vector2f dir = aGoalPos - aPos;
		Normalize(dir);
		aPos = aPos + dir * aSpeedWithDelta;
	}

	template<typename T>//made for floating point precision
	bool ApproxEqual(const Vector2<T>& aFirst, const Vector2<T>& aSecond, T aMargin = 0.0001)
	{
		return (aFirst.x + aMargin > aSecond.x && 
				aFirst.x - aMargin < aSecond.x &&

				aFirst.y + aMargin > aSecond.y && 
				aFirst.y - aMargin < aSecond.y);// == false;
	}

	template<typename T>
	Vector2<T> Lerp(Vector2<T> &aFirstPoint, Vector2<T> &aSecondPoint, float aAlpha)
	{
		return aFirstPoint + ((aSecondPoint - aFirstPoint) * aAlpha);
	}

	template<typename T>
	T Dot(const Vector2<T>& aVector, const Vector2<T>& aSecondVector)
	{
		return ((aVector.x * aSecondVector.x) + (aVector.y * aSecondVector.y));
	}

	template<typename T>
	Vector2<T> GetNormalized(const Vector2<T>& aVector)
	{
		Vector2<T> returnVal(aVector);
		returnVal = returnVal / Length(returnVal);

		return (returnVal);
	}

	template<typename T>
	Vector2<T> operator+(const Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		Vector2<T> returnVal(aLeftVector);

		returnVal.x += aRightVector.x;
		returnVal.y += aRightVector.y;

		return (returnVal);
	}
	
	template<typename T>
	bool operator!=(const Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		if(aLeftVector.x == aRightVector.x && aLeftVector.y == aRightVector.y)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	Vector2<T> operator-(const Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		Vector2<T> returnVal(aLeftVector);

		returnVal.x -= aRightVector.x;
		returnVal.y -= aRightVector.y;

		return (returnVal);
	}

	template<typename T>
	Vector2<T> operator*(const Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		Vector2<T> returnVal(aLeftVector);

		returnVal.x *= aRightVector.x;
		returnVal.y *= aRightVector.y;

		return (returnVal);
	}

	template<typename T>
	Vector2<T> operator/(const Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		Vector2<T> returnVal(aLeftVector);

		returnVal.x /= aRightVector.x;
		returnVal.y /= aRightVector.y;

		return (returnVal);
	}

	template<typename T>
	Vector2<T>& operator+=(Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		aLeftVector.x += aRightVector.x;
		aLeftVector.y += aRightVector.y;

		return (aLeftVector);
	}

	template<typename T>
	Vector2<T>& operator-=(Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		aLeftVector.x -= aRightVector.x;
		aLeftVector.y -= aRightVector.y;

		return (aLeftVector);
	}

	template<typename T>
	Vector2<T>& operator*=(Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		aLeftVector.x *= aRightVector.x;
		aLeftVector.y *= aRightVector.y;

		return (aLeftVector);
	}

	template<typename T>
	Vector2<T>& operator/=(Vector2<T>& aLeftVector, const Vector2<T>& aRightVector)
	{
		aLeftVector.x /= aRightVector.x;
		aLeftVector.y /= aRightVector.y;

		return (aLeftVector);
	}

	template<typename T>
	Vector2<T> operator*(const Vector2<T>& aLeftVector, const float aScalar)
	{
		Vector2<T> returnVal(aLeftVector);

		returnVal.x *= aScalar;
		returnVal.y *= aScalar;

		return (returnVal);
	}

	template<typename T>
	Vector2<T> operator/(const Vector2<T>& aLeftVector, const float aScalar)
	{
		Vector2<T> returnVal(aLeftVector);

		returnVal.x /= aScalar;
		returnVal.y /= aScalar;

		return (returnVal);
	}

	template<typename T>
	Vector2<T>& operator/=(Vector2<T>& aLeftVector, const float aScalar)
	{
		aLeftVector.x /= aScalar;
		aLeftVector.y /= aScalar;

		return (aLeftVector);
	}

	template<typename T>
	Vector2<T>& operator*=(Vector2<T>& aLeftVector, const float aScalar)
	{
		aLeftVector.x *= aScalar;
		aLeftVector.y *= aScalar;

		return (aLeftVector);
	}



};



#endif