#include "CameraClass.h"

#define _USE_MATH_DEFINES

#include <math.h>


CameraClass::CameraClass()
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(const Vector3f& pos)
{
	m_Position = pos;
}
void CameraClass::SetRotation(const Vector3f& rot)
{
	m_Rotation = rot;
}

void CameraClass::Render()
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
	float pitch;
	float yaw;
	float roll;

	pitch = m_Rotation.x * (M_PI / 180.f);
	yaw = m_Rotation.y * (M_PI / 180.f);
	roll = m_Rotation.z * (M_PI / 180.f);

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	m_ViewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}
void CameraClass::GetViewMatrix(DirectX::XMMATRIX& matrix)
{
	matrix = m_ViewMatrix;
}

DirectX::XMFLOAT3 CameraClass::GetPositionDX()
{
	return DirectX::XMFLOAT3(m_Position.x,
							 m_Position.y,
							 m_Position.z);
}
DirectX::XMFLOAT3 CameraClass::GetRotationDX()
{
	return DirectX::XMFLOAT3(m_Rotation.x,
							 m_Rotation.y,
							 m_Rotation.z);
}

Vector3f CameraClass::GetPosition()
{
	return m_Position;
}
Vector3f CameraClass::GetRotation()
{
	return m_Rotation;
}