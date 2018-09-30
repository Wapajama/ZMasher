#pragma once
#include "mathdefines.h"
#include "ZMVector4.h"
namespace ZMasher
{

	///////////////////////////////////////////////////////////////////////////////////
	/*			VECTOR4f		*/
	///////////////////////////////////////////////////////////////////////////////////

	ZM_ALWAYS_INLINE const Vector4f Vector4Add(const Vector4f& operand1, const Vector4f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_add_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector4f(operand1.x + operand2.x,
						operand1.y + operand2.y,
						operand1.z + operand2.z,
						operand1.w + operand2.w);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector4f Vector4Sub(const Vector4f& operand1, const Vector4f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_sub_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector4f(operand1.x - operand2.x,
						operand1.y - operand2.y,
						operand1.z - operand2.z,
						operand1.w - operand2.w);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector4f Vector4Mul(const Vector4f& operand1, const Vector4f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_mul_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector4f(operand1.x * operand2.x,
						operand1.y * operand2.y,
						operand1.z * operand2.z,
						operand1.w * operand2.w);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector4f Vector4Div(const Vector4f& operand1, const Vector4f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_div_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector4f(operand1.x / operand2.x,
						operand1.y / operand2.y,
						operand1.z / operand2.z,
						operand1.w / operand2.w);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector4f Vector4MulScalar(const Vector4f& operand, const float scalar)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_mul_ps(_mm_set_ps1(scalar), operand.m_Data));
#else
		return Vector4f(operand.x * scalar,
					   operand.y * scalar,
					   operand.z * scalar,
					   operand.w * scalar);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector4f Vector4DivScalar(const Vector4f& operand, const float scalar)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_div_ps(_mm_set_ps1(scalar), operand.m_Data));
#else
		return Vector4f(operand.x / scalar,
					   operand.y / scalar,
					   operand.z / scalar,
					   operand.w / scalar);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector3f Cross(const Vector3f& operand1, const Vector3f& operand2);
	ZM_ALWAYS_INLINE const Vector4f Cross4(const Vector4f& operand1, const Vector4f& operand2)//NOTE: Doesn't actually use Vector4:s, just used for SIMD operations
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4f(_mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(operand1.m_Data, operand1.m_Data, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(operand2.m_Data, operand2.m_Data, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(operand1.m_Data, operand1.m_Data, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(operand2.m_Data, operand2.m_Data, _MM_SHUFFLE(3, 0, 2, 1)))
			));
#else
		return Vector4f(Cross(Vector3f(operand1), Vector3f(operand2)));
#endif // ZM_MATH_USE_SIMD

	}

	ZM_ALWAYS_INLINE Vector4f Lerp(const Vector4f& leftOperand, const Vector4f& rightOperand, const float percentage)
	{
		return leftOperand + ((rightOperand - (leftOperand))* percentage);
	}

	ZM_ALWAYS_INLINE const float Dot(const Vector4f& leftOperand, const Vector4f& rightOperand)
	{
#ifdef ZM_MATH_USE_SIMD
		const Vector4f temp = Vector4f(leftOperand*rightOperand);
		return (temp.x + temp.y + temp.z + temp.w);
#else
		return	leftOperand.x * rightOperand.x +
			leftOperand.y * rightOperand.y +
			leftOperand.z * rightOperand.z +
			leftOperand.w * rightOperand.w;
#endif // ZM_MATH_USE_SIMD

	}

	ZM_ALWAYS_INLINE const float Length2(const Vector4f& operand)
	{
		return ((operand.x * operand.x) +
				(operand.y * operand.y) +
				(operand.z * operand.z) +
				(operand.w * operand.w));
	}

	ZM_ALWAYS_INLINE const float Length(const Vector4f& operand)
	{
		return sqrt(ZMasher::Length2(operand));
	}

	inline const Vector4f Vector4f::operator+(const Vector4f& operand)const
	{
		return Vector4Add(*this, operand);
	}

	inline void Vector4f::operator+=(const Vector4f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector4Add(*this, operand);
#else
		x += operand.x;
		y += operand.y;
		z += operand.z;
		w += operand.w;
#endif // ZM_MATH_USE_SIMD
	}

	inline const Vector4f Vector4f::operator-(const Vector4f& operand)const
	{
		return Vector4Sub(*this, operand);
	}
	inline void Vector4f::operator-=(const Vector4f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector4Sub(*this, operand);
#else
		x -= operand.x;
		y -= operand.y;
		z -= operand.z;
		w -= operand.w;
#endif // ZM_MATH_USE_SIMD

	}

	inline const Vector4f Vector4f::operator*(const Vector4f& operand)const
	{
		return Vector4Mul(*this, operand);
	}
	inline void Vector4f::operator*=(const Vector4f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector4Mul(*this, operand);
#else
		x *= operand.x;
		y *= operand.y;
		z *= operand.z;
		w *= operand.w;
#endif // ZM_MATH_USE_SIMD
	}

	inline const Vector4f Vector4f::operator/(const Vector4f& operand)const
	{
		return Vector4Div(*this, operand);
	}
	inline void Vector4f::operator/=(const Vector4f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector4Div(*this, operand);
#else
		x /= operand.x;
		y /= operand.y;
		z /= operand.z;
		w /= operand.w;
#endif // ZM_MATH_USE_SIMD
	}

	inline const Vector4f Vector4f::operator*(const float operand)const
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector4MulScalar(*this, operand);
#else
		return Vector4f(x * operand,
						y * operand,
						z * operand,
						w * operand);
#endif // ZM_MATH_USE_SIMD

	}
	inline void Vector4f::operator*=(const float operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector4MulScalar(*this, operand);
#else
		x *= operand;
		y *= operand;
		z *= operand;
		w *= operand;
#endif // ZM_MATH_USE_SIMD

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

	inline Vector4f::Vector4f()
	{
		x = CAST(0);
		y = CAST(0);
		z = CAST(0);
		w = CAST(0);
	}

	inline Vector4f::Vector4f(const Vector4f& copy)
	{
		//m_Data = copy.m_Data;
		x = copy.x;
		y = copy.y;
		z = copy.z;
		w = copy.w;
	}

	inline Vector4f::Vector4f(const Vector3f& vector3, const float aw)
	{
		//m_Data = vector3.m_Data;
		x = vector3.x;
		y = vector3.y;
		z = vector3.z;
		w = aw;
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
		//m_Data = data;
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

	inline Vector3f Vector4f::ToVector3f()const { return Vector3f(x, y, z); }

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

	///////////////////////////////////////////////////////////////////////////////////
	/*			VECTOR3f		*/
	///////////////////////////////////////////////////////////////////////////////////
#ifdef ZM_MATH_USE_SIMD //don't use it for Vector3f, seems broken af atm
#undef ZM_MATH_USE_SIMD
#endif // ZM_MATH_USE_SIMD

	ZM_ALWAYS_INLINE const Vector3f Vector3Add(const Vector3f& operand1, const Vector3f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(_mm_add_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector3f(operand1.x + operand2.x,
						operand1.y + operand2.y,
						operand1.z + operand2.z);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector3f Vector3Sub(const Vector3f& operand1, const Vector3f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(_mm_sub_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector3f(operand1.x - operand2.x,
						operand1.y - operand2.y,
						operand1.z - operand2.z);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector3f Vector3Mul(const Vector3f& operand1, const Vector3f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(_mm_mul_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector3f(operand1.x * operand2.x,
						operand1.y * operand2.y,
						operand1.z * operand2.z);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector3f Vector3Div(const Vector3f& operand1, const Vector3f& operand2)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(_mm_div_ps(operand1.m_Data, operand2.m_Data));
#else
		return Vector3f(operand1.x / operand2.x,
						operand1.y / operand2.y,
						operand1.z / operand2.z);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector3f Vector3MulScalar(const Vector3f& operand, const float scalar)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(_mm_mul_ps(_mm_set_ps1(scalar), operand.m_Data));
#else
		return Vector3f(operand.x * scalar,
					   operand.y * scalar,
					   operand.z * scalar);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE const Vector3f Vector3DivScalar(const Vector3f& operand, const float scalar)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(_mm_div_ps(_mm_set_ps1(scalar), operand.m_Data));
#else
		return Vector3f(operand.x / scalar,
					   operand.y / scalar,
					   operand.z / scalar);
#endif // ZM_MATH_USE_SIMD
	}

	ZM_ALWAYS_INLINE Vector3f Lerp(const Vector3f& leftOperand, const Vector3f& rightOperand, const float percentage)
	{
		return leftOperand + ((rightOperand - (leftOperand))* percentage);
	}

	ZM_ALWAYS_INLINE const float Dot(const Vector3f& leftOperand, const Vector3f& rightOperand)
	{
		return	leftOperand.x * rightOperand.x +
			leftOperand.y * rightOperand.y +
			leftOperand.z * rightOperand.z;
	}

	ZM_ALWAYS_INLINE const Vector3f Cross(const Vector3f& leftOperand, const Vector3f& rightOperand)
	{
#ifdef ZM_MATH_USE_SIMD
		return Vector3f(Cross4(Vector4f(leftOperand), Vector4f(rightOperand)));//Cross4 uses SIMD, when the define is used
#else
		return Vector3f(
			leftOperand.y * rightOperand.z - leftOperand.z * rightOperand.y,
			leftOperand.z * rightOperand.x - leftOperand.x * rightOperand.z,
			leftOperand.x * rightOperand.y - leftOperand.y - rightOperand.x);
#endif // ZM_MATH_USE_SIMD

	}

	ZM_ALWAYS_INLINE const float Length2(const Vector3f& operand)
	{
		return ((operand.x * operand.x) +
				(operand.y * operand.y) +
				(operand.z * operand.z));
	}

	ZM_ALWAYS_INLINE const float Length(const Vector3f& operand)
	{
		return sqrt(ZMasher::Length2(operand));
	}

#define SET_W_TO_ZERO //m_Data.m128_f32[3] = CAST(0)//else it will create garbage data

	inline Vector3f::Vector3f()
	{
		x = CAST(0);
		y = CAST(0);
		z = CAST(0);
		SET_W_TO_ZERO;
	}

	inline Vector3f::Vector3f(const Vector3f& copy)
	{
		//m_Data = copy.m_Data;
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	inline Vector3f::Vector3f(const Vector4f& vector4)
	{
		//m_Data = vector4.m_Data;
		x = vector4.x;
		y = vector4.y;
		z = vector4.z;

	}

	inline Vector3f::Vector3f(const float ax, const float ay, const float az)
	{
		x = ax;
		y = ay;
		z = az;
		SET_W_TO_ZERO;
	}

	//inline Vector3f::Vector3f(const __m128& data)
	//{
	//	m_Data = data;
	//}

	inline Vector3f::Vector3f(float* arrayPtr)
	{
		assert(arrayPtr != nullptr && "Vector3f: Constructorargument is null!");
		x = arrayPtr[0];
		y = arrayPtr[1];
		z = arrayPtr[2];
		SET_W_TO_ZERO;
	}

	inline Vector3f::~Vector3f()
	{
	}

	inline const Vector3f Vector3f::operator+(const Vector3f& operand)const
	{
		return Vector3Add(*this, operand);
	}

	inline void Vector3f::operator+=(const Vector3f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector3Add(*this, operand);
#else
		x += operand.x;
		y += operand.y;
		z += operand.z;
#endif // ZM_MATH_USE_SIMD

	}

	inline const Vector3f Vector3f::operator-(const Vector3f& operand)const
	{
		return Vector3Sub(*this, operand);
	}
	inline void Vector3f::operator-=(const Vector3f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector3Sub(*this, operand);
#else
		x -= operand.x;
		y -= operand.y;
		z -= operand.z;
#endif // ZM_MATH_USE_SIMD

	}

	inline const Vector3f Vector3f::operator*(const Vector3f& operand)const
	{
		return Vector3Mul(*this, operand);
	}
	inline void Vector3f::operator*=(const Vector3f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector3Mul(*this, operand);
#else
		x *= operand.x;
		y *= operand.y;
		z *= operand.z;
#endif // ZM_MATH_USE_SIMD
	}

	inline const Vector3f Vector3f::operator/(const Vector3f& operand)const
	{
		return Vector3Div(*this, operand);
	}
	inline void Vector3f::operator/=(const Vector3f& operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector3Div(*this, operand);
#else
		x /= operand.x;
		y /= operand.y;
		z /= operand.z;
#endif // ZM_MATH_USE_SIMD
	}

	inline const Vector3f Vector3f::operator*(const float operand)const
	{
		return Vector3MulScalar(*this, operand);
	}
	inline void Vector3f::operator*=(const float operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector3MulScalar(*this, operand);
#else	
		x *= operand;
		y *= operand;
		z *= operand;
#endif // ZM_MATH_USE_SIMD

	}

	inline const Vector3f Vector3f::operator/(const float operand)const
	{
		return Vector3DivScalar(*this, operand);
	}
	inline void Vector3f::operator/=(const float operand)
	{
#ifdef ZM_MATH_USE_SIMD
		*this = Vector3DivScalar(*this, operand);
#else
		x /= operand;
		y /= operand;
		z /= operand;
#endif // ZM_MATH_USE_SIMD

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