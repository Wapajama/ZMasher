#include "Camera.h"
#include "ZMasherUtilities.h"

using namespace ZMasher;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(const Vector3f& pos)
{
	m_WorldOrientation.SetTranslation(Vector4f(pos, 1.f));
}
void Camera::SetRotation(const Vector3f& rot)
{
	m_Rotation = rot;
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMFLOAT3 up;
	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	DirectX::XMFLOAT3 position;
	position.x = m_WorldOrientation.GetTranslation().x;
	position.y = m_WorldOrientation.GetTranslation().y;
	position.z = m_WorldOrientation.GetTranslation().z;

	DirectX::XMFLOAT3 lookAt;
	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&up);
	DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR lookAtVector = DirectX::XMLoadFloat3(&lookAt);
	
	float pitch =	GetRadians(m_Rotation.x);
	float yaw =		GetRadians(m_Rotation.y);
	float roll =	GetRadians(m_Rotation.z);

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector =	DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
	lookAtVector =	DirectX::XMVectorAdd(positionVector, lookAtVector);
	upVector =		DirectX::XMVector3TransformCoord(upVector, rotationMatrix);	

	m_ViewMatrix =	DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(DirectX::XMMATRIX& matrix)
{
	matrix = m_ViewMatrix;
}

DirectX::XMFLOAT3 Camera::GetPositionDX()
{
	return DirectX::XMFLOAT3(m_WorldOrientation.GetTranslation().x,
							 m_WorldOrientation.GetTranslation().y,
							 m_WorldOrientation.GetTranslation().z);
}
DirectX::XMFLOAT3 Camera::GetRotationDX()
{
	return DirectX::XMFLOAT3(m_WorldOrientation.GetTranslation().x,
							 m_WorldOrientation.GetTranslation().y,
							 m_WorldOrientation.GetTranslation().z);
}

Vector3f Camera::GetPosition()
{
	return Vector3f(m_WorldOrientation.GetTranslation());
}

Vector3f Camera::GetRotation()
{
	return m_Rotation;
}