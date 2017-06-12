#pragma once

#include <dvec.h>
#include "ZMVector.h"

namespace ZMasher
{
	class __declspec(align(16)) Matrix44f
	{
	public:

		//constructors

		inline Matrix44f()
		{
			*this = Identity();
		}

		inline Matrix44f(const Matrix44f& copy)
		{
			//printf("testderp %ld %ld %ld\n", 
			//	   (intptr_t)(&m_Data[0]) & 0xF, 
			//	   (intptr_t)(&copy.m_Data[0]) & 0xF,
			//	   (intptr_t)(&m_Data[1]) & 0xF);
			//printf("testderp %ld %ld %ld\n", 
			//	   (intptr_t)(this), 
			//	   (intptr_t)(&copy),
			//	   (intptr_t)(&m_Data[0]));
			//
			//intptr_t test1 = (intptr_t)(&m_Data[0]) & 0xF;
			//intptr_t test2 = (intptr_t)(&copy.m_Data[0]) & 0xF;
			//intptr_t test3 = (intptr_t)(&m_Data[1]) & 0xF;

			m_Data[0] = copy.m_Data[0];
			m_Data[1] = copy.m_Data[1];
			m_Data[2] = copy.m_Data[2];
			m_Data[3] = copy.m_Data[3];
		}

		inline Matrix44f(const float arg_1,
				  const float arg_2,
				  const float arg_3,
				  const float arg_4,
				  const float arg_5,
				  const float arg_6,
				  const float arg_7,
				  const float arg_8,
				  const float arg_9,
				  const float arg_10,
				  const float arg_11,
				  const float arg_12,
				  const float arg_13,
				  const float arg_14,
				  const float arg_15,
				  const float arg_16)
		{
			m11 = arg_1;
			m12 = arg_2;
			m13 = arg_3;
			m14 = arg_4;
			m21 = arg_5;
			m22 = arg_6;
			m23 = arg_7;
			m24 = arg_8;
			m31 = arg_9;
			m32 = arg_10;
			m33 = arg_11;
			m34 = arg_12;
			m41 = arg_13;
			m42 = arg_14;
			m43 = arg_15;
			m44 = arg_16;
		}

		inline Matrix44f(float* array)
		{
			m11 = array[0];
			m12 = array[1];
			m13 = array[2];
			m14 = array[3];
			m21 = array[4];
			m22 = array[5];
			m23 = array[6];
			m24 = array[7];
			m31 = array[8];
			m32 = array[9];
			m33 = array[10];
			m34 = array[11];
			m41 = array[12];
			m42 = array[13];
			m43 = array[14];
			m44 = array[15];
		}

		inline Matrix44f(const __m128& x,
				  const __m128& y,
				  const __m128& z,
				  const __m128& w)
		{
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = w;
		}

		inline ~Matrix44f() {}

		//member variables
		union
		{
			struct
			{
				float m11, m12, m13, m14;
				float m21, m22, m23, m24;
				float m31, m32, m33, m34;
				float m41, m42, m43, m44;
			};
			__m128 m_Data[4];//used for SIMD operations
			Vector4f m_Vectors[4];
			float m_Elements[4][4];//used for pros B)
			float m_ElementsSingle[16];
		};


		//operators

			//operators with matrix

		inline Matrix44f operator+(const Matrix44f& operand)const;
		inline Matrix44f operator-(const Matrix44f& operand)const;
		inline Matrix44f operator*(const Matrix44f& operand)const;

		inline Matrix44f& operator+=(const Matrix44f& operand);
		inline Matrix44f& operator-=(const Matrix44f& operand);
		inline Matrix44f& operator*=(const Matrix44f& operand);

		inline Matrix44f operator~()const;

			//operators with vector

		inline Vector4f operator*(const Vector4f& operand)const;

		inline Vector4f& operator*=(const Vector4f& operand);

			//operators with scalar

		Matrix44f operator*(const float scalar)const;

		Matrix44f& operator*=(const float scalar);

		inline float& operator[](const int index);

		//utility

		inline void SetTranslation(const Vector4f& operand);
		inline Vector4f GetTranslation()const;

		inline void RotateX(const float radians);
		inline void RotateY(const float radians);
		inline void RotateZ(const float radians);
		
		inline float GetRotationX()const;
		inline float GetRotationY()const;
		inline float GetRotationZ()const;

		inline Vector4f GetVectorForward()const;
		inline Vector4f GetVectorUp()const;
		inline Vector4f GetVectorLeft()const;

		inline static const Matrix44f Identity();

		inline static const Matrix44f CreateRotationMatrixX(const float radians);
		inline static const Matrix44f CreateRotationMatrixY(const float radians);
		inline static const Matrix44f CreateRotationMatrixZ(const float radians);

		inline static const Matrix44f CreateRotationMatrixAroundAxis(const Vector4f& vector, const float radians);
	};

}

#include "ZMMatrixMath.inl"