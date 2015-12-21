#include "Camera.h"
#include "ZMasherUtilities.h"

using namespace ZMasher;

Camera::Camera()
{
	m_WorldOrientation = Matrix44f::Identity();
	m_ProjectionMatrix = Matrix44f::Identity();
}

Camera::~Camera()
{
}

void Camera::SetPosition(const Vector3f& pos)
{
	m_WorldOrientation.SetTranslation(Vector4f(pos, 1.f));
}

void Camera::UpdateViewMatrix()
{
}

void Camera::GetViewMatrix(DirectX::XMMATRIX& matrix)
{

	matrix.r[0] = m_WorldOrientation.m_Data[0];
	matrix.r[1] = m_WorldOrientation.m_Data[1];
	matrix.r[2] = m_WorldOrientation.m_Data[2];
	matrix.r[3] = m_WorldOrientation.m_Data[3];
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
