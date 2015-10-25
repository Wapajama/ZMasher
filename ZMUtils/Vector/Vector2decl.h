#ifndef VECTOR2DECL_H
#define VECTOR2DECL_H

namespace CommonUtilities
{
	template<typename T>
	class Vector2
	{
	public:

		Vector2			();
		Vector2			(const Vector2<T>&);
		Vector2			(const T, const T);
		~Vector2		();
		void Set(T aX, T aY);
		Vector2<T> Absolute();

		union
		{
			T myX;
			T x;
			T u;
		};
		union
		{
			T myY;
			T y;
			T v;
		};
	};
	template<typename T>
	void Vector2<T>::Set(T aX, T aY)
	{
		x = aX;
		y = aY;
	}

	template<typename T>
	Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}

	template<typename T>
	Vector2<T>::Vector2(const Vector2<T>& aVector)
	{
		x = aVector.x;
		y = aVector.y;
	}

	template<typename T>
	Vector2<T>::Vector2(const T aX, const T aY)
	{
		x = aX;
		y = aY;
	}

	template<typename T>
	Vector2<T> Vector2<T>::Absolute()
	{
		return Vector2<T>(abs(myX), abs(myY));
	}

	template<typename T>
	Vector2<T>::~Vector2()
	{
	}

	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<int> Vector2i;
	typedef Vector2<unsigned short> Vector2us;
};

#endif