#pragma once

#include <smmintrin.h>
#include <dvec.h>
#include <Windows.h>

//#define ZMASHER_VECTOR3_TEMPLATE_DECL template<typename T>
#define ZMASHER_VECTOR3_TEMPLATE_DECL 
//#define T float
#define CAST(variable) static_cast<float>(variable)

namespace ZMasher
{

	ZMASHER_VECTOR3_TEMPLATE_DECL
	class __declspec(align(16)) Vector3f
	{
	public:
		inline Vector3f();
		inline Vector3f(const Vector3f& copy);
		inline Vector3f(const __m128& data);
		inline Vector3f(const float x, const float y, const float z);
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

		inline Vector3f operator+(const Vector3f& operand);
		inline void operator+=(const Vector3f& operand);
	};

	ZMASHER_VECTOR3_TEMPLATE_DECL
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

	ZMASHER_VECTOR3_TEMPLATE_DECL
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

	ZMASHER_VECTOR3_TEMPLATE_DECL
	inline Vector3f::~Vector3f()
	{
	}

	ZMASHER_VECTOR3_TEMPLATE_DECL
	inline Vector3f Vector3f::operator+(const Vector3f& term)
	{
		//_mm_add_epi16

		//return Vector3f	(	CAST(this->x + term.x),
		//					CAST(this->y + term.y),
		//					CAST(this->z + term.z));
		return Vector3f( _mm_add_ps(m_Data, term.m_Data));
	}

	inline void Vector3f::operator+=(const Vector3f& operand)
	{
		
		//6-8 SIMD
		m_Data = _mm_add_ps(m_Data, operand.m_Data);

		//3-5 
		//x += operand.x;
		//y += operand.y;
		//z += operand.z;

		//0-2
		//x = x + operand.x;
		//y = y + operand.y;
		//z = z + operand.z;
	}
}

//typedef ZMasher::Vector3<float> __declspec(align(16)) Vector3f;
//typedef ZMasher::Vector3<double> __declspec(align(32)) Vector3d;
