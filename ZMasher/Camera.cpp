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
	m_Position = pos;
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
	position.x = m_Position.x;
	position.y = m_Position.y;
	position.z = m_Position.z;

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
	return DirectX::XMFLOAT3(m_Position.x,
							 m_Position.y,
							 m_Position.z);
}
DirectX::XMFLOAT3 Camera::GetRotationDX()
{
	return DirectX::XMFLOAT3(m_Rotation.x,
							 m_Rotation.y,
							 m_Rotation.z);
}

Vector3f Camera::GetPosition()
{
	return m_Position;
}

Vector3f Camera::GetRotation()
{
	return m_Rotation;
}