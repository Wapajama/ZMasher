#ifndef VECTOR3DECL_H
#define VECTOR3DECL_H

namespace ZMasher
{
	template<typename T>
	class Vector3
	{
	public:
		Vector3		();
		Vector3		(const Vector3<T>&);
		Vector3		(const T, const T, const T);
		~Vector3		();
		void Set	(const T aX, const T aY, const T aZ);
		
		T operator[](const short) const;
		T& operator[](const short);

		union
		{
			T myX;
			T x;
			T r;
			T myR;
		};

		union
		{
			T myY;
			T y;
			T g;
			T myG;
		};

		union
		{
			T myZ;
			T z;
			T b;
			T myB;
		};
	};

	template<typename T>
	Vector3<T>::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	template<typename T>
	Vector3<T>::Vector3(const Vector3<T>& aVector)
	{
		x = aVector.x;
		y = aVector.y;
		z = aVector.z;
	}

	template<typename T>
	Vector3<T>::Vector3(const T aX, const T aY, const T aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	template<typename T>
	Vector3<T>::~Vector3()
	{
	}

	template<typename T>
	void Vector3<T>::Set(const T aX, const T aY, const T aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}


	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
};

#endif