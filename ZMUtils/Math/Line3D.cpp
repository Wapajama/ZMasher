#include "Line3D.h"

namespace ZMasher
{

	Line3D::Line3D()
	{
	}

	Line3D::Line3D(const ZMasher::Vector3f& first_point,
				   const ZMasher::Vector3f& second_point)
	{
		m_Points[0] = first_point;
		m_Points[1] = second_point;
	}

	Line3D::~Line3D()
	{
	}
}