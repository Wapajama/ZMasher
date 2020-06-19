#include "Camera.h"
#include <ZMUtils/Utility/ZMasherUtilities.h>
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
	m_Translation = pos;
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
	m_ProjectionMatrix.Set(DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane));
}

void Camera::GetOrientation(DirectX::XMMATRIX& matrix)
{
	m_Orientation.Get(matrix);
}

void Camera::GetProjectionMatrix(DirectX::XMMATRIX& matrix)
{
	m_ProjectionMatrix.Get(matrix);
}

Vector3f Camera::GetPosition()
{
	return m_Translation;
}

ZMasher::Matrix44f Camera::GetWorldOrientation()const
{
	return m_Orientation;
}

void Camera::RotateX(const float radians)
{
	m_Orientation *= ZMasher::Matrix44f::CreateRotationMatrixX(radians);
}

void Camera::RotateY(const float radians)
{
	m_Orientation *= ZMasher::Matrix44f::CreateRotationMatrixY(radians);
}

void Camera::RotateZ(const float radians)
{
	m_Orientation.RotateZ(radians);
}
