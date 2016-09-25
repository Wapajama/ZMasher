#pragma once
#include "Debugging/ZMDebugger.h"
#include "ZMVector3.h"//shouldn't be using relative paths but it's a fucking cunt towards me atm :S

namespace ZMasher
{
	class Line3D
	{
	public:
		Line3D();
		Line3D(const ZMasher::Vector3f& first_point, 
			   const ZMasher::Vector3f& second_point);
		~Line3D();

		inline ZMasher::Vector3f GetPoint(const char index)const;
		inline void SetPoint(const char index, const ZMasher::Vector3f& point);

	private:
		ZMasher::Vector3f m_Points[2];
	};

	inline ZMasher::Vector3f Line3D::GetPoint(const char index)const
	{
		ASSERT(index >= 0 && index < 2, "Out of bounds!");
		return m_Points[index];
	}
	inline void Line3D::SetPoint(const char index, const ZMasher::Vector3f& point)
	{
		ASSERT(index >= 0 && index < 2, "Out of bounds!");
		m_Points[index] = point;
	}
}