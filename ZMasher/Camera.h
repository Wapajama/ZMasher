#pragma once
#include <DirectXMath.h>
#include <ZMVector.h>
#include <ZMMatrix44.h>

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const ZMasher::Vector3f& pos);

	void UpdateViewMatrix();
	void GetViewMatrix(DirectX::XMMATRIX& matrix);

	DirectX::XMFLOAT3 GetPositionDX();
	ZMasher::Vector3f GetPosition();

	void RotateX(const float radians);
	void RotateY(const float radians);
	void RotateZ(const float radians);

private:

	DirectX::XMMATRIX m_ViewMatrix;

	ZMasher::Matrix44f m_WorldOrientation;
	ZMasher::Matrix44f m_ProjectionMatrix;
	
};

