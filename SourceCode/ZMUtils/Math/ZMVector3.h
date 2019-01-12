#pragma once

#include <math.h>
#include <dvec.h>
#include <Windows.h>
#include <assert.h>

#define CAST(variable) static_cast<float>(variable)
namespace ZMasher
{
	class Vector4f;
}

namespace ZMasher
{
	class __declspec(align(16)) Vector3f
	{
	public:
		inline Vector3f();
		inline Vector3f(const Vector3f& copy);
		inline explicit Vector3f(const Vector4f&);
		//inline explicit Vector3f(const __m128& data);
		inline explicit Vector3f(const float x, const float y, const float z);
		inline explicit Vector3f(float* arrayPtr);
		inline ~Vector3f();

		union
		{
			//__m128 m_Data;
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


}
