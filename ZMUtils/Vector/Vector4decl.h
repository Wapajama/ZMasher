#ifndef VECTOR4DECL_H
#define VECTOR4DECL_H

namespace CommonUtilities
{
	template<typename T>
	class Vector4
	{
	public:
		Vector4		();
		Vector4		(const Vector4<T>&);
		Vector4		(const T, const T, const T, const T);
		~Vector4	();

		void operator=(const Vector3<T>& aRightVector);
		union
		{
			T myX;
			T x;
			T myR;
			T r;
		};

		union
		{
			T myY;
			T y;
			T myG;
			T g;
		};

		union
		{
			T myZ;
			T z;
			T myB;
			T b;
		};

		union
		{
			T myW;
			T w;
			T myA;
			T a;
		};
	};

	template<typename T>
	Vector4<T>::Vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	template<typename T>
	Vector4<T>::Vector4(const Vector4<T>& aVector)
	{
		x = aVector.x;
		y = aVector.y;
		z = aVector.z;
		w = aVector.w;
	}

	template<typename T>
	Vector4<T>::Vector4(const T aX, const T aY, const T aZ, const T aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}

	template<typename T>
	Vector4<T>::~Vector4()
	{
	}

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
};

#endif