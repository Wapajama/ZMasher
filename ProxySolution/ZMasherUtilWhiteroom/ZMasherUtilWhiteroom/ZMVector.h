#pragma once

#define ZMASHER_VECTOR3_TEMPLATE_DECL template<typename T>
#define CAST(variable) static_cast<T>(variable)
namespace ZMasher
{

	ZMASHER_VECTOR3_TEMPLATE_DECL
	class Vector3
	{
	public:
		Vector3();
		~Vector3();

		T x;
		T y;
		T z;
				
	};

	ZMASHER_VECTOR3_TEMPLATE_DECL
	Vector3<T>::Vector3()
	{
		x = CAST(0);
		y = CAST(0);
		z = CAST(0);
	}

	ZMASHER_VECTOR3_TEMPLATE_DECL
	Vector3<T>~Vector3()
	{
	}

}