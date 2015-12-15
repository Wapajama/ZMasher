#pragma once

#include <math.h>
#include <dvec.h>
#include <Windows.h>
#include <assert.h>

#define CAST(variable) static_cast<float>(variable)

namespace ZMasher
{
	class __declspec(align(16)) Vector3f
	{
	public:
		inline Vector3f();
		inline Vector3f(const Vector3f& copy);
		inline explicit Vector3f(const __m128& data);
		inline explicit Vector3f(const float x, const float y, const float z);
		inline explicit Vector3f(float* arrayPtr);
		inline ~Vector3f();

		union
		{
			__m128 m_Data;
			struct
			{
				float x;
				float y;
				float z;
			};
		};

		inline const Vector3f operator+(const Vector3f& operand)const;
		inline void operator+=(const Vector3f& operand);

		inline const Vector3f operator-(const Vector3f& operand)const;
		inline void operator-=(const Vector3f& operand);

		inline const Vector3f operator*(const Vector3f& operand)const;
		inline void operator*=(const Vector3f& operand);

		inline const Vector3f operator/(const Vector3f& operand)const;
		inline void operator/=(const Vector3f& operand);

		inline const Vector3f operator*(const float operand)const;
		inline void operator*=(const float operand);

		inline const Vector3f operator/(const float operand)const;
		inline void operator/=(const float operand);

		inline const Vector3f Lerp(const Vector3f& operand, const float percentage)const;
		inline float Dot(const Vector3f& operand)const;
		inline const Vector3f Cross(const Vector3f& operand)const;
		inline void Normalize();
		inline const Vector3f GetNormal()const;
		inline const float Length()const;

	};

	static __forceinline Vector3f Lerp(const Vector3f& leftOperand,
									   const Vector3f& rightOperand, const float percentage)
	{
		return leftOperand + ((rightOperand - (leftOperand))* percentage);
	}

	static __forceinline const float Dot(const Vector3f& leftOperand,
										 const Vector3f& rightOperand)
	{
		return	leftOperand.x * rightOperand.x +
			leftOperand.y * rightOperand.y +
			leftOperand.z * rightOperand.z;
	}

	static __forceinline Vector3f Cross(const Vector3f& leftOperand,
										const Vector3f& rightOperand)
	{
		return Vector3f(
			leftOperand.y * rightOperand.z - leftOperand.z * rightOperand.y,
			leftOperand.z * rightOperand.x - leftOperand.x * rightOperand.z,
			leftOperand.x * rightOperand.y - leftOperand.y - rightOperand.x);
	}

	static __forceinline const float Length2(const Vector3f& operand)
	{
		return ((operand.x * operand.x) +
				(operand.y * operand.y) +
				(operand.z * operand.z));
	}

	static __forceinline const float Length(const Vector3f& operand)
	{
		return sqrt(ZMasher::Length2(operand));
	}

	inline Vector3f::Vector3f()
	{
		x = CAST(0);
		y = CAST(0);
		z = CAST(0);
	}

	inline Vector3f::Vector3f(const Vector3f& copy)
	{
		m_Data = copy.m_Data;
	}

	inline Vector3f::Vector3f(const float ax, const float ay, const float az)
	{
		x = ax;
		y = ay;
		z = az;
	}

	inline Vector3f::Vector3f(const __m128& data)
	{
		m_Data = data;
	}

	inline Vector3f::Vector3f(float* arrayPtr)
	{
		assert(arrayPtr != nullptr && "Vector3f: Constructorargument is null!");
		x = arrayPtr[0];
		y = arrayPtr[1];
		z = arrayPtr[2];
	}

	inline Vector3f::~Vector3f()
	{
	}

	inline const Vector3f Vector3f::operator+(const Vector3f& operand)const
	{
		return Vector3f(x + operand.x,
						y + operand.y,
						z + operand.z);
	}

	inline void Vector3f::operator+=(const Vector3f& operand)
	{
		x += operand.x;
		y += operand.y;
		z += operand.z;
	}

	inline const Vector3f Vector3f::operator-(const Vector3f& operand)const
	{
		return Vector3f(x - operand.x,
						y - operand.y,
						z - operand.z);
	}
	inline void Vector3f::operator-=(const Vector3f& operand)
	{
		x -= operand.x;
		y -= operand.y;
		z -= operand.z;
	}

	inline const Vector3f Vector3f::operator*(const Vector3f& operand)const
	{
		return Vector3f(x * operand.x,
						y * operand.y,
						z * operand.z);
	}
	inline void Vector3f::operator*=(const Vector3f& operand)
	{
		x *= operand.x;
		y *= operand.y;
		z *= operand.z;
	}

	inline const Vector3f Vector3f::operator/(const Vector3f& operand)const
	{
		return Vector3f(x / operand.x,
						y / operand.y,
						z / operand.z);
	}
	inline void Vector3f::operator/=(const Vector3f& operand)
	{
		x /= operand.x;
		y /= operand.y;
		z /= operand.z;
	}

	inline const Vector3f Vector3f::operator*(const float operand)const
	{
		return Vector3f(x * operand,
						y * operand,
						z * operand);
	}
	inline void Vector3f::operator*=(const float operand)
	{
		x *= operand;
		y *= operand;
		z *= operand;
	}

	inline const Vector3f Vector3f::operator/(const float operand)const
	{
		return Vector3f(x / operand,
						y / operand,
						z / operand);
	}
	inline void Vector3f::operator/=(const float operand)
	{
		x /= operand;
		y /= operand;
		z /= operand;
	}

	inline const Vector3f Vector3f::Lerp(const Vector3f& operand, const float percentage)const
	{
		return ZMasher::Lerp(*this, operand, percentage);
	}
	inline float Vector3f::Dot(const Vector3f& operand)const
	{
		return ZMasher::Dot(*this, operand);
	}
	inline const Vector3f Vector3f::Cross(const Vector3f& operand)const
	{
		return ZMasher::Cross(*this, operand);
	}
	inline void Vector3f::Normalize()
	{
		*this /= this->Length();
	}
	inline const float Vector3f::Length()const
	{
		return ZMasher::Length(*this);
	}
	inline const Vector3f Vector3f::GetNormal()const
	{
		return (*this) / this->Length();
	}
}
