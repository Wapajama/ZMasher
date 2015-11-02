#pragma once
#include <DirectXMath.h>
#include <Vector\Vector.h>

class CameraClass
{
public:
	CameraClass();
	~CameraClass();

	void SetPosition(const Vector3f& pos);
	void SetRotation(const Vector3f& rot);

	void UpdateViewMatrix();
	void GetViewMatrix(DirectX::XMMATRIX& matrix);

	DirectX::XMFLOAT3 GetPositionDX();
	DirectX::XMFLOAT3 GetRotationDX();

	Vector3f GetPosition();
	Vector3f GetRotation();

private:
	
	Vector3f m_Position;
	Vector3f m_Rotation;
	DirectX::XMMATRIX m_ViewMatrix;

};

