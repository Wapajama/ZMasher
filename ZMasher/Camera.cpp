#include "Camera.h"
#include "ZMasherUtilities.h"

using namespace ZMasher;

Camera::Camera(const ZMasher::Vector2<int>& window_size)
{
	m_WorldOrientation = Matrix44f::Identity();
	m_ProjectionMatrix = Matrix44f::Identity();
	m_FOV = GetRadians(80);
	m_NearPlane = 0.1;
	m_FarPlane = 1000;

	this->SetWindowSize(window_size);

}

Camera::~Camera()
{
}

void Camera::SetPosition(const Vector3f& pos)
{
	m_WorldOrientation.SetTranslation(Vector4f(pos, 1.f));
}

void Camera::SetWindowSize(const ZMasher::Vector2<int>& size)
{
	m_WindowSize = size;
	m_AspectRatio = static_cast<float>(size.x) /
		static_cast<float>(size.y);
}

void Camera::UpdateProjMatrix()
{
	m_ProjectionMatrix = Matrix44f(&DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane).r[0].m128_f32[0]);
}

void Camera::GetWorldOrientation(DirectX::XMMATRIX& matrix)
{
	matrix.r[0] = m_WorldOrientation.m_Data[0];
	matrix.r[1] = m_WorldOrientation.m_Data[1];
	matrix.r[2] = m_WorldOrientation.m_Data[2];
	matrix.r[3] = m_WorldOrientation.m_Data[3];
}

void Camera::GetProjectionMatrix(DirectX::XMMATRIX& matrix)
{
	matrix.r[0] = m_ProjectionMatrix.m_Data[0];
	matrix.r[1] = m_ProjectionMatrix.m_Data[1];
	matrix.r[2] = m_ProjectionMatrix.m_Data[2];
	matrix.r[3] = m_ProjectionMatrix.m_Data[3];
}

DirectX::XMFLOAT3 Camera::GetPositionDX()
{
	return DirectX::XMFLOAT3(m_WorldOrientation.GetTranslation().x,
							 m_WorldOrientation.GetTranslation().y,
							 m_WorldOrientation.GetTranslation().z);
}

Vector3f Camera::GetPosition()
{
	return Vector3f(m_WorldOrientation.GetTranslation());
}

void Camera::RotateX(const float radians)
{
	m_WorldOrientation.RotateX(radians);
}

void Camera::RotateY(const float radians)
{
	m_WorldOrientation.RotateY(radians);
}

void Camera::RotateZ(const float radians)
{
	m_WorldOrientation.RotateZ(radians);
}