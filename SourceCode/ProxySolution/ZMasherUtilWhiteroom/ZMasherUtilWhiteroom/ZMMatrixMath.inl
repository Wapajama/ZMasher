#pragma once
#include "ZMMatrix44.h"
#include "mathdefines.h"

//#define ZM_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )
namespace ZMasher
{
	extern ZM_ALWAYS_INLINE const Vector4f Vector4MulScalar(const Vector4f& operand, const float scalar);

	ZM_ALWAYS_INLINE const Vector4f Vector4MulMatrix44(const Vector4f& operand, const Matrix44f& matrix)
	{
		Vector4f result;

#ifdef ZM_MATH_USE_SIMD

		Vector4f x = Vector4f(_mm_mul_ps(operand.m_Data, Vector4f(matrix.m11, matrix.m21, matrix.m31, matrix.m41).m_Data));
		Vector4f y = Vector4f(_mm_mul_ps(operand.m_Data, Vector4f(matrix.m12, matrix.m22, matrix.m32, matrix.m42).m_Data));
		Vector4f z = Vector4f(_mm_mul_ps(operand.m_Data, Vector4f(matrix.m13, matrix.m23, matrix.m33, matrix.m43).m_Data));
		Vector4f w = Vector4f(_mm_mul_ps(operand.m_Data, Vector4f(matrix.m14, matrix.m24, matrix.m34, matrix.m44).m_Data));


		result.x += x.x += x.y += x.z += x.w;
		result.y += y.x += y.y += y.z += y.w;
		result.z += z.x += z.y += z.z += z.w;
		result.w += w.x += w.y += w.z += w.w;
#else

#endif // ZM_MATH_USE_SIMD

		

		return result;
	}

	ZM_ALWAYS_INLINE const Matrix44f Matrix44MulVector4(const Matrix44f& matrix, const Vector4f& operand)
	{
		assert(false);
	}

	ZM_ALWAYS_INLINE const Matrix44f Matrix44Add(const Matrix44f& operand1, const Matrix44f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD

		return Matrix44f(
			_mm_add_ps(operand1.m_Data[0], operand2.m_Data[0]),
			_mm_add_ps(operand1.m_Data[1], operand2.m_Data[1]),
			_mm_add_ps(operand1.m_Data[2], operand2.m_Data[2]),
			_mm_add_ps(operand1.m_Data[3], operand2.m_Data[3]));
#else
		return Matrix44f(
			operand1.m11 + operand2.m11, operand1.m12 + operand2.m12, operand1.m13 + operand2.m13, operand1.m14 + operand2.m14,
			operand1.m21 + operand2.m21, operand1.m22 + operand2.m22, operand1.m23 + operand2.m23, operand1.m24 + operand2.m24,
			operand1.m31 + operand2.m31, operand1.m32 + operand2.m32, operand1.m33 + operand2.m33, operand1.m34 + operand2.m34,
			operand1.m41 + operand2.m41, operand1.m42 + operand2.m42, operand1.m43 + operand2.m43, operand1.m44 + operand2.m44);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Matrix44f Matrix44Sub(const Matrix44f& operand1, const Matrix44f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD

		return Matrix44f(
			_mm_sub_ps(operand1.m_Data[0], operand2.m_Data[0]),
			_mm_sub_ps(operand1.m_Data[1], operand2.m_Data[1]),
			_mm_sub_ps(operand1.m_Data[2], operand2.m_Data[2]),
			_mm_sub_ps(operand1.m_Data[3], operand2.m_Data[3]));
#else
		return Matrix44f(
			operand1.m11 - operand2.m11, operand1.m12 - operand2.m12, operand1.m13 - operand2.m13, operand1.m14 - operand2.m14,
			operand1.m21 - operand2.m21, operand1.m22 - operand2.m22, operand1.m23 - operand2.m23, operand1.m24 - operand2.m24,
			operand1.m31 - operand2.m31, operand1.m32 - operand2.m32, operand1.m33 - operand2.m33, operand1.m34 - operand2.m34,
			operand1.m41 - operand2.m41, operand1.m42 - operand2.m42, operand1.m43 - operand2.m43, operand1.m44 - operand2.m44);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Matrix44f Matrix44Mul(const Matrix44f& operand1, const Matrix44f& operand2)
	{
		Matrix44f result;
#ifdef ZM_MATH_USE_SIMD

		//0
		__m128 w = operand1.m_Data[0];

		__m128 x =	_mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 y =	_mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 z =	_mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w =			_mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, operand2.m_Data[0]);
		y = _mm_mul_ps(y, operand2.m_Data[1]);
		z = _mm_mul_ps(z, operand2.m_Data[2]);
		w = _mm_mul_ps(w, operand2.m_Data[3]);

		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);

		result.m_Data[0] = x;

		//1
		w = operand1.m_Data[1];

		x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, operand2.m_Data[0]);
		y = _mm_mul_ps(y, operand2.m_Data[1]);
		z = _mm_mul_ps(z, operand2.m_Data[2]);
		w = _mm_mul_ps(w, operand2.m_Data[3]);

		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);

		result.m_Data[1] = x;

		//2
		w = operand1.m_Data[2];

		x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, operand2.m_Data[0]);
		y = _mm_mul_ps(y, operand2.m_Data[1]);
		z = _mm_mul_ps(z, operand2.m_Data[2]);
		w = _mm_mul_ps(w, operand2.m_Data[3]);

		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);

		result.m_Data[2] = x;

		//3
		w = operand1.m_Data[3];

		x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, operand2.m_Data[0]);
		y = _mm_mul_ps(y, operand2.m_Data[1]);
		z = _mm_mul_ps(z, operand2.m_Data[2]);
		w = _mm_mul_ps(w, operand2.m_Data[3]);

		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);

		result.m_Data[3] = x;
#else
		//0
		float x = operand1.m_Elements[0][0];
		float y = operand1.m_Elements[0][1];
		float z = operand1.m_Elements[0][2];
		float w = operand1.m_Elements[0][3];

		result.m_Elements[0][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		result.m_Elements[0][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		result.m_Elements[0][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		result.m_Elements[0][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

		//1
		x = operand1.m_Elements[1][0];
		y = operand1.m_Elements[1][1];
		z = operand1.m_Elements[1][2];
		w = operand1.m_Elements[1][3];

		result.m_Elements[1][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		result.m_Elements[1][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		result.m_Elements[1][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		result.m_Elements[1][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

		//2
		x = operand1.m_Elements[2][0];
		y = operand1.m_Elements[2][1];
		z = operand1.m_Elements[2][2];
		w = operand1.m_Elements[2][3];

		result.m_Elements[2][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		result.m_Elements[2][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		result.m_Elements[2][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		result.m_Elements[2][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

		//3
		x = operand1.m_Elements[3][0];
		y = operand1.m_Elements[3][1];
		z = operand1.m_Elements[3][2];
		w = operand1.m_Elements[3][3];

		result.m_Elements[3][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		result.m_Elements[3][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		result.m_Elements[3][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		result.m_Elements[3][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

#endif // !ZM_MATH_USE_SIMD
		return result;
	}

	ZM_ALWAYS_INLINE const Matrix44f Matrix44SMulScal(const Matrix44f& operand, const float scalar)
	{
		Matrix44f result;

		result.m_Data[0] = Vector4MulScalar(Vector4f(operand.m_Data[0]), scalar).m_Data;
		result.m_Data[1] = Vector4MulScalar(Vector4f(operand.m_Data[1]), scalar).m_Data;
		result.m_Data[2] = Vector4MulScalar(Vector4f(operand.m_Data[2]), scalar).m_Data;
		result.m_Data[3] = Vector4MulScalar(Vector4f(operand.m_Data[3]), scalar).m_Data;
		
		return result;
	}


	ZM_ALWAYS_INLINE void Matrix44AddDir(Matrix44f& operand1, const Matrix44f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD

		operand1.m_Data[0] = _mm_add_ps(operand1.m_Data[0], operand2.m_Data[0]);
		operand1.m_Data[1] = _mm_add_ps(operand1.m_Data[1], operand2.m_Data[1]);
		operand1.m_Data[2] = _mm_add_ps(operand1.m_Data[2], operand2.m_Data[2]);
		operand1.m_Data[3] = _mm_add_ps(operand1.m_Data[3], operand2.m_Data[3]);
#else
		operand1.m11 += operand2.m11;
		operand1.m12 += operand2.m12;
		operand1.m13 += operand2.m13;
		operand1.m14 += operand2.m14;

		operand1.m21 += operand2.m21;
		operand1.m22 += operand2.m22;
		operand1.m23 += operand2.m23;
		operand1.m24 += operand2.m24;

		operand1.m31 += operand2.m31;
		operand1.m32 += operand2.m32;
		operand1.m33 += operand2.m33;
		operand1.m34 += operand2.m34;

		operand1.m41 += operand2.m41;
		operand1.m42 += operand2.m42;
		operand1.m43 += operand2.m43;
		operand1.m44 += operand2.m44;
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE void Matrix44SubDir(Matrix44f& operand1, const Matrix44f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD

		operand1.m_Data[0] = _mm_sub_ps(operand1.m_Data[0], operand2.m_Data[0]);
		operand1.m_Data[1] = _mm_sub_ps(operand1.m_Data[1], operand2.m_Data[1]);
		operand1.m_Data[2] = _mm_sub_ps(operand1.m_Data[2], operand2.m_Data[2]);
		operand1.m_Data[3] = _mm_sub_ps(operand1.m_Data[3], operand2.m_Data[3]);
#else
		operand1.m11 -= operand2.m11;
		operand1.m12 -= operand2.m12;
		operand1.m13 -= operand2.m13;
		operand1.m14 -= operand2.m14;

		operand1.m21 -= operand2.m21;
		operand1.m22 -= operand2.m22;
		operand1.m23 -= operand2.m23;
		operand1.m24 -= operand2.m24;

		operand1.m31 -= operand2.m31;
		operand1.m32 -= operand2.m32;
		operand1.m33 -= operand2.m33;
		operand1.m34 -= operand2.m34;

		operand1.m41 -= operand2.m41;
		operand1.m42 -= operand2.m42;
		operand1.m43 -= operand2.m43;
		operand1.m44 -= operand2.m44;
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE void Matrix44MulDir(Matrix44f& operand1, const Matrix44f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		assert(false && "not implemented!");
#else
		//0
		float x = operand1.m_Elements[0][0];
		float y = operand1.m_Elements[0][1];
		float z = operand1.m_Elements[0][2];
		float w = operand1.m_Elements[0][3];

		operand1.m_Elements[0][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		operand1.m_Elements[0][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		operand1.m_Elements[0][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		operand1.m_Elements[0][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

		//1
		x = operand1.m_Elements[1][0];
		y = operand1.m_Elements[1][1];
		z = operand1.m_Elements[1][2];
		w = operand1.m_Elements[1][3];

		operand1.m_Elements[1][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		operand1.m_Elements[1][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		operand1.m_Elements[1][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		operand1.m_Elements[1][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

		//2
		x = operand1.m_Elements[2][0];
		y = operand1.m_Elements[2][1];
		z = operand1.m_Elements[2][2];
		w = operand1.m_Elements[2][3];

		operand1.m_Elements[2][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		operand1.m_Elements[2][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		operand1.m_Elements[2][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		operand1.m_Elements[2][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

		//3
		x = operand1.m_Elements[3][0];
		y = operand1.m_Elements[3][1];
		z = operand1.m_Elements[3][2];
		w = operand1.m_Elements[3][3];

		operand1.m_Elements[3][0] = (operand2.m_Elements[0][0] * x) + (operand2.m_Elements[1][0] * y) + (operand2.m_Elements[2][0] * z) + (operand2.m_Elements[3][0] * w);
		operand1.m_Elements[3][1] = (operand2.m_Elements[0][1] * x) + (operand2.m_Elements[1][1] * y) + (operand2.m_Elements[2][1] * z) + (operand2.m_Elements[3][1] * w);
		operand1.m_Elements[3][2] = (operand2.m_Elements[0][2] * x) + (operand2.m_Elements[1][2] * y) + (operand2.m_Elements[2][2] * z) + (operand2.m_Elements[3][2] * w);
		operand1.m_Elements[3][3] = (operand2.m_Elements[0][3] * x) + (operand2.m_Elements[1][3] * y) + (operand2.m_Elements[2][3] * z) + (operand2.m_Elements[3][3] * w);

#endif // !ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE void Matrix44MulScalDir(Matrix44f& operand, const float scalar)
	{
		operand.m_Data[0] = Vector4MulScalar(Vector4f(operand.m_Data[0]), scalar).m_Data;
		operand.m_Data[1] = Vector4MulScalar(Vector4f(operand.m_Data[1]), scalar).m_Data;
		operand.m_Data[2] = Vector4MulScalar(Vector4f(operand.m_Data[2]), scalar).m_Data;
		operand.m_Data[3] = Vector4MulScalar(Vector4f(operand.m_Data[3]), scalar).m_Data;
	}

	//rip these guys out to a designated .inl file

	inline Matrix44f Matrix44f::operator+(const Matrix44f& operand)const
	{
		return Matrix44f(Matrix44Add(*this, operand));
	}

	inline Matrix44f Matrix44f::operator-(const Matrix44f& operand)const
	{
		return Matrix44f(Matrix44Sub(*this, operand));
	}

	inline Matrix44f Matrix44f::operator*(const Matrix44f& operand)const
	{
		return Matrix44f(Matrix44Mul(*this, operand));
	}


	inline Matrix44f& Matrix44f::operator+=(const Matrix44f& operand)
	{
		Matrix44AddDir(*this, operand);
	}

	inline Matrix44f& Matrix44f::operator-=(const Matrix44f& operand)
	{
		Matrix44SubDir(*this, operand);
	}

	inline Matrix44f& Matrix44f::operator*=(const Matrix44f& operand)
	{
		Matrix44MulDir(*this, operand);
	}


	inline Vector4f Matrix44f::operator*(const Vector4f& operand)const
	{
		Vector4f result;

		

		return result;
	}

	inline Vector4f& Matrix44f::operator*=(const Vector4f& operand)
	{

	}


	Matrix44f Matrix44f::operator*(const float scalar)const
	{
		return Matrix44SMulScal(*this, scalar);
	}

	Matrix44f& Matrix44f::operator*=(const float scalar)
	{
		Matrix44MulScalDir(*this, scalar);
		return *this;
	}


	inline void Matrix44f::SetTranslation(const Vector4f& operand)
	{
		m14 = operand.x;
		m24 = operand.y;
		m34 = operand.z;
		m44 = operand.w;//think twice before changing m44!
	}

	inline Vector4f Matrix44f::GetTranslation()const
	{
		return Vector4f(m14, m24, m34, m44);
	}


	inline void Matrix44f::RotateX(const float radians)
	{
		(*this) *= CreateRotationMatrixX(radians);
	}

	inline void Matrix44f::RotateY(const float radians)
	{
		(*this) *= CreateRotationMatrixY(radians);
	}

	inline void Matrix44f::RotateZ(const float radians)
	{
		(*this) *= CreateRotationMatrixZ(radians);
	}


	inline const Matrix44f Matrix44f::Identity()
	{
		return Matrix44f(1.f, 0.f, 0.f, 0.f,
						 0.f, 1.f, 0.f, 0.f,
						 0.f, 0.f, 1.f, 0.f,
						 0.f, 0.f, 0.f, 1.f);
	}


	inline const Matrix44f Matrix44f::CreateRotationMatrixX(const float radians)
	{
		Matrix44f rotationMatrix = Identity();
		const float cosine = cos(radians);
		const float sine = sin(radians);

		rotationMatrix.m22 = cosine;
		rotationMatrix.m23 = -sine;
		rotationMatrix.m32 = sine;
		rotationMatrix.m33 = cosine;

		return rotationMatrix;
	}

	inline const Matrix44f Matrix44f::CreateRotationMatrixY(const float radians)
	{
		Matrix44f rotationMatrix = Identity();
		const float cosine = cos(radians);
		const float sine = sin(radians);

		rotationMatrix.m11 = cosine;
		rotationMatrix.m13 = sine;
		rotationMatrix.m31 = -sine;
		rotationMatrix.m33 = cosine;

		return rotationMatrix;
	}

	inline const Matrix44f Matrix44f::CreateRotationMatrixZ(const float radians)
	{
		Matrix44f rotationMatrix = Identity();
		const float cosine = cos(radians);
		const float sine = sin(radians);

		rotationMatrix.m11 = cosine;
		rotationMatrix.m12 = -sine;
		rotationMatrix.m21 = sine;
		rotationMatrix.m22 = cosine;

		return rotationMatrix;
	}

}