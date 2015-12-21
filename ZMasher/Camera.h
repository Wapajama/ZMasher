#pragma once
#include <DirectXMath.h>
#include <ZMVector.h>

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const ZMasher::Vector3f& pos);
	void SetRotation(const ZMasher::Vector3f& rot);

	void UpdateViewMatrix();
	void GetViewMatrix(DirectX::XMMATRIX& matrix);

	DirectX::XMFLOAT3 GetPositionDX();
	DirectX::XMFLOAT3 GetRotationDX();

	ZMasher::Vector3f GetPosition();
	ZMasher::Vector3f GetRotation();

private:
	
	ZMasher::Vector3f m_Position;
	ZMasher::Vector3f m_Rotation;
	DirectX::XMMATRIX m_ViewMatrix;

};

