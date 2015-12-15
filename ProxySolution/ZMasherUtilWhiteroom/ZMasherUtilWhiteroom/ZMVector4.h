#pragma once

#include <math.h>
#include <dvec.h>
//#include <Windows.h>
#include <assert.h>
#define CAST(variable) static_cast<float>(variable)

namespace ZMasher
{
	class __declspec(align(16)) Vector4f
	{
	public:
		inline Vector4f();
		inline Vector4f(const Vector4f& copy);
		inline explicit Vector4f(const __m128& data);
		inline explicit Vector4f(const float x, const float y, const float z, const float w);
		inline explicit Vector4f(float* arrayPtr);
		inline ~Vector4f();

		union
		{
			__m128 m_Data;
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};

		inline const Vector4f operator+(const Vector4f& operand)const;
		inline void operator+=(const Vector4f& operand);

		inline const Vector4f operator-(const Vector4f& operand)const;
		inline void operator-=(const Vector4f& operand);

		inline const Vector4f operator*(const Vector4f& operand)const;
		inline void operator*=(const Vector4f& operand);

		inline const Vector4f operator/(const Vector4f& operand)const;
		inline void operator/=(const Vector4f& operand);

		inline const Vector4f operator*(const float operand)const;
		inline void operator*=(const float operand);

		inline const Vector4f operator/(const float operand)const;
		inline void operator/=(const float operand);

		inline const Vector4f Lerp(const Vector4f& operand, const float percentage)const;
		inline float Dot(const Vector4f& operand)const;
		inline const Vector4f Cross(const Vector4f& operand)const;
		inline void Normalize();
		inline const Vector4f GetNormal()const;
		inline const float Length()const;

	};

	static __forceinline Vector4f Lerp(const Vector4f& leftOperand,
									   const Vector4f& rightOperand, const float percentage)
	{
		return leftOperand + ((rightOperand - (leftOperand))* percentage);
	}

	static __forceinline const float Dot(const Vector4f& leftOperand,
										 const Vector4f& rightOperand)
	{
		return	leftOperand.x * rightOperand.x +
			leftOperand.y * rightOperand.y +
			leftOperand.z * rightOperand.z +
			leftOperand.w * rightOperand.w;
	}

	static __forceinline const float Length2(const Vector4f& operand)
	{
		return ((operand.x * operand.x) +
				(operand.y * operand.y) +
				(operand.z * operand.z) +
				(operand.w * operand.w));
	}

	static __forceinline const float Length(const Vector4f& operand)
	{
		return sqrt(ZMasher::Length2(operand));
	}

	inline Vector4f::Vector4f()
	{
		x = CAST(0);
		y = CAST(0);
		z = CAST(0);
		w = CAST(0);
	}

	inline Vector4f::Vector4f(const Vector4f& copy)
	{
		m_Data = copy.m_Data;
	}

	inline Vector4f::Vector4f(const float ax, const float ay, const float az, const float aw)
	{
		x = ax;
		y = ay;
		z = az;
		w = aw;
	}

	inline Vector4f::Vector4f(const __m128& data)
	{
		m_Data = data;
	}

	inline Vector4f::Vector4f(float* arrayPtr)
	{
		assert(arrayPtr != nullptr && "Vector4f: Constructorargument is null!");
		x = arrayPtr[0];
		y = arrayPtr[1];
		z = arrayPtr[2];
		w = arrayPtr[3];
	}

	inline Vector4f::~Vector4f()
	{
	}

	inline const Vector4f Vector4f::operator+(const Vector4f& operand)const
	{
		return Vector4f(x + operand.x,
						y + operand.y,
						z + operand.z,
						w + operand.w);
	}

	inline void Vector4f::operator+=(const Vector4f& operand)
	{
		x += operand.x;
		y += operand.y;
		z += operand.z;
		w += operand.w;
	}

	inline const Vector4f Vector4f::operator-(const Vector4f& operand)const
	{
		return Vector4f(x - operand.x,
						y - operand.y,
						z - operand.z,
						w - operand.w);
	}
	inline void Vector4f::operator-=(const Vector4f& operand)
	{
		x -= operand.x;
		y -= operand.y;
		z -= operand.z;
		w -= operand.w;
	}

	inline const Vector4f Vector4f::operator*(const Vector4f& operand)const
	{
		return Vector4f(x * operand.x,
						y * operand.y,
						z * operand.z,
						w * operand.w);
	}
	inline void Vector4f::operator*=(const Vector4f& operand)
	{
		x *= operand.x;
		y *= operand.y;
		z *= operand.z;
		w *= operand.w;
	}

	inline const Vector4f Vector4f::operator/(const Vector4f& operand)const
	{
		return Vector4f(x / operand.x,
						y / operand.y,
						z / operand.z,
						w / operand.w);
	}
	inline void Vector4f::operator/=(const Vector4f& operand)
	{
		x /= operand.x;
		y /= operand.y;
		z /= operand.z;
		w /= operand.w;
	}

	inline const Vector4f Vector4f::operator*(const float operand)const
	{
		return Vector4f(x * operand,
						y * operand,
						z * operand,
						w * operand);
	}
	inline void Vector4f::operator*=(const float operand)
	{
		x *= operand;
		y *= operand;
		z *= operand;
		w *= operand;
	}

	inline const Vector4f Vector4f::operator/(const float operand)const
	{
		return Vector4f(x / operand,
						y / operand,
						z / operand,
						w / operand);
	}
	inline void Vector4f::operator/=(const float operand)
	{
		x /= operand;
		y /= operand;
		z /= operand;
		w /= operand;
	}

	inline const Vector4f Vector4f::Lerp(const Vector4f& operand, const float percentage)const
	{
		return ZMasher::Lerp(*this, operand, percentage);
	}
	inline float Vector4f::Dot(const Vector4f& operand)const
	{
		return ZMasher::Dot(*this, operand);
	}

	inline void Vector4f::Normalize()
	{
		*this /= this->Length();
	}
	inline const float Vector4f::Length()const
	{
		return ZMasher::Length(*this);
	}
	inline const Vector4f Vector4f::GetNormal()const
	{
		return (*this) / this->Length();
	}
}