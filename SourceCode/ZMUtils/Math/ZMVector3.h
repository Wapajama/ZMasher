#pragma once

#include <math.h>
// #include <dvec.h>
#include <Windows.h>
#include <assert.h>

#define CAST(variable) static_cast<float>(variable)
namespace ZMasher
{
	class Vector4f;
}

namespace ZMasher
{
	class Vector3f
	{
	public:
		Vector3f();
		Vector3f(const Vector3f& copy);
		Vector3f(const Vector4f& vector4);
		Vector3f(const float ax, const float ay, const float az);
		Vector3f(float* arrayPtr);
		~Vector3f();

		union
		{
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
