#include "Camera.h"
#include "ZMasherUtilities.h"
#include <iostream>
using namespace ZMasher;

Camera::Camera(const ZMasher::Vector2<int>& window_size): m_XRotation(0), m_YRotation(0)
{
	m_Orientation = Matrix44f::Identity();
	m_ProjectionMatrix = Matrix44f::Identity();
	m_FOV = GetRadians(80);
	m_NearPlane = 0.1;
	m_FarPlane = 100000;

	this->SetWindowSize(window_size);

}

Camera::~Camera()
{
}

void Camera::SetPosition(const Vector3f& pos)
{
	m_Orientation.SetTranslation(Vector4f(pos, m_Orientation.GetTranslation().w));
}

void Camera::SetOrientation(const ZMasher::Matrix44f& orientation)
{
	m_Orientation = orientation;
}

void Camera::SetWindowSize(const ZMasher::Vector2i& size)
{
	m_WindowSize = size;
	m_AspectRatio = static_cast<float>(size.x) /
		static_cast<float>(size.y);
}

const ZMasher::Vector2i Camera::GetWindowSize()const
{
	return m_WindowSize;
}

void Camera::UpdateProjMatrix()
{
	m_ProjectionMatrix = Matrix44f(&DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane).r[0].m128_f32[0]);
}

void Camera::GetOrientation(DirectX::XMMATRIX& matrix)
{
	//matrix.r[0] = m_Orientation.m_Data[0];
	//matrix.r[1] = m_Orientation.m_Data[1];
	//matrix.r[2] = m_Orientation.m_Data[2];
	//matrix.r[3] = m_Orientation.m_Data[3];
	matrix.r[0].m128_f32[0] = m_Orientation.m_Elements[0][0];
	matrix.r[0].m128_f32[1] = m_Orientation.m_Elements[0][1];
	matrix.r[0].m128_f32[2] = m_Orientation.m_Elements[0][2];
	matrix.r[0].m128_f32[3] = m_Orientation.m_Elements[0][3];

	matrix.r[1].m128_f32[0] = m_Orientation.m_Elements[1][0];
	matrix.r[1].m128_f32[1] = m_Orientation.m_Elements[1][1];
	matrix.r[1].m128_f32[2] = m_Orientation.m_Elements[1][2];
	matrix.r[1].m128_f32[3] = m_Orientation.m_Elements[1][3];

	matrix.r[2].m128_f32[0] = m_Orientation.m_Elements[2][0];
	matrix.r[2].m128_f32[1] = m_Orientation.m_Elements[2][1];
	matrix.r[2].m128_f32[2] = m_Orientation.m_Elements[2][2];
	matrix.r[2].m128_f32[3] = m_Orientation.m_Elements[2][3];

	matrix.r[3].m128_f32[0] = m_Orientation.m_Elements[3][0];
	matrix.r[3].m128_f32[1] = m_Orientation.m_Elements[3][1];
	matrix.r[3].m128_f32[2] = m_Orientation.m_Elements[3][2];
	matrix.r[3].m128_f32[3] = m_Orientation.m_Elements[3][3];
}

void Camera::GetProjectionMatrix(DirectX::XMMATRIX& matrix)
{
	//matrix.r[0] = m_ProjectionMatrix.m_Data[0];
	//matrix.r[1] = m_ProjectionMatrix.m_Data[1];
	//matrix.r[2] = m_ProjectionMatrix.m_Data[2];
	//matrix.r[3] = m_ProjectionMatrix.m_Data[3];

	matrix.r[0].m128_f32[0] = m_ProjectionMatrix.m_Elements[0][0];
	matrix.r[0].m128_f32[1] = m_ProjectionMatrix.m_Elements[0][1];
	matrix.r[0].m128_f32[2] = m_ProjectionMatrix.m_Elements[0][2];
	matrix.r[0].m128_f32[3] = m_ProjectionMatrix.m_Elements[0][3];
							  
	matrix.r[1].m128_f32[0] = m_ProjectionMatrix.m_Elements[1][0];
	matrix.r[1].m128_f32[1] = m_ProjectionMatrix.m_Elements[1][1];
	matrix.r[1].m128_f32[2] = m_ProjectionMatrix.m_Elements[1][2];
	matrix.r[1].m128_f32[3] = m_ProjectionMatrix.m_Elements[1][3];
							  
	matrix.r[2].m128_f32[0] = m_ProjectionMatrix.m_Elements[2][0];
	matrix.r[2].m128_f32[1] = m_ProjectionMatrix.m_Elements[2][1];
	matrix.r[2].m128_f32[2] = m_ProjectionMatrix.m_Elements[2][2];
	matrix.r[2].m128_f32[3] = m_ProjectionMatrix.m_Elements[2][3];
							  
	matrix.r[3].m128_f32[0] = m_ProjectionMatrix.m_Elements[3][0];
	matrix.r[3].m128_f32[1] = m_ProjectionMatrix.m_Elements[3][1];
	matrix.r[3].m128_f32[2] = m_ProjectionMatrix.m_Elements[3][2];
	matrix.r[3].m128_f32[3] = m_ProjectionMatrix.m_Elements[3][3];
}

Vector3f Camera::GetPosition()
{
	return Vector3f(m_Orientation.GetTranslation());
}

ZMasher::Matrix44f Camera::GetWorldOrientation()const
{
	return m_Orientation;
}

void Camera::GetWorldOrientationaTest(ZMasher::Matrix44f& matrix)const
{
	//m_Orientation.m_Data[0];
	//matrix.m_Data[0];
	//matrix.m_Data[0].m128_i32[0];
	//matrix.m_Data[0].m128_i32[0] = 1;
	//matrix.m_Data[0].m128_i32[1] = INT_MAX;
	//matrix.m_Data[0].m128_i32[2] = 2;
	//matrix.m_Data[0].m128_i32[3] = INT_MAX-1;
	//

	//matrix.m_Data[0].m128_f32[0] = m_Orientation.m_Data[0].m128_f32[0];
	//matrix.m_Data[0].m128_f32[1] = m_Orientation.m_Data[0].m128_f32[1];
	//matrix.m_Data[0].m128_f32[2] = m_Orientation.m_Data[0].m128_f32[2];
	//matrix.m_Data[0].m128_f32[3] = m_Orientation.m_Data[0].m128_f32[3];

	//printf("testderp %ld %ld %ld\n", 
	//		(intptr_t)(&matrix.m_Data[0]) & 0xF, 
	//		(intptr_t)(&m_Orientation.m_Data[0]) & 0xF,
	//		(intptr_t)(&matrix.m_Data[1]) & 0xF);
	//printf("testderp %ld %ld %ld\n", 
	//		(intptr_t)(&matrix.m_Data[0]), 
	//		(intptr_t)(&m_Orientation),
	//		(intptr_t)(&matrix.m_Data[1]));

	//matrix.m_Data[0] = m_Orientation.m_Data[0];
	//matrix.m_Data[1] = m_Orientation.m_Data[1];
	//matrix.m_Data[2] = m_Orientation.m_Data[2];
	//matrix.m_Data[3] = m_Orientation.m_Data[3];

	matrix = m_Orientation;


	//printf("testderp AFTER\n");
}

void Camera::RotateX(const float radians)
{
	const ZMasher::Vector4f temp_pos = m_Orientation.GetTranslation();
	m_Orientation.SetTranslation(ZMasher::Vector4f(0, 0, 0, temp_pos.w));
	m_Orientation *= ZMasher::Matrix44f::CreateRotationMatrixX(radians);
	m_Orientation.SetTranslation(temp_pos);
}

void Camera::RotateY(const float radians)
{
	const ZMasher::Vector4f temp_pos = m_Orientation.GetTranslation();
	m_Orientation.SetTranslation(ZMasher::Vector4f(0, 0, 0, temp_pos.w));
	m_Orientation *= ZMasher::Matrix44f::CreateRotationMatrixY(radians);
	m_Orientation.SetTranslation(temp_pos);
}

void Camera::RotateZ(const float radians)
{
	m_Orientation.RotateZ(radians);
}
