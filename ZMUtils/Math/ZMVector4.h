#pragma once

#include <math.h>
#include <dvec.h>
#include <assert.h>

#define CAST(variable) static_cast<float>(variable)
namespace ZMasher
{
	class Vector3f;
}

namespace ZMasher
{
	class __declspec(align(16)) Vector4f 
	{
	public:
		inline Vector4f();
		inline Vector4f(const Vector4f& copy);
		inline explicit Vector4f(const Vector3f& vector3, const float aw = 0.f);
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
		inline void Normalize();
		inline const Vector4f GetNormal()const;
		inline const float Length()const;

	};

}

#include "ZMVectorMath.inl"