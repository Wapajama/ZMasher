#pragma once
#include <DirectXMath.h>
#include <Math/ZMVector.h>
#include <Math/ZMMatrix44.h>
#include <Math/Vector2decl.h>

class Camera
{
public:
	Camera(const ZMasher::Vector2<int>& window_size);
	~Camera();

	void SetPosition(const ZMasher::Vector3f& pos);

	void UpdateProjMatrix();
	void GetWorldOrientation(DirectX::XMMATRIX& matrix);
	void GetProjectionMatrix(DirectX::XMMATRIX& matrix);

	DirectX::XMFLOAT3 GetPositionDX();
	ZMasher::Vector3f GetPosition();

	void SetWindowSize(const ZMasher::Vector2<int>& size);

	void RotateX(const float radians);
	void RotateY(const float radians);
	void RotateZ(const float radians);

private:

	float m_FOV;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

	ZMasher::Vector2<int> m_WindowSize;

	ZMasher::Matrix44f m_WorldOrientation;
	ZMasher::Matrix44f m_ProjectionMatrix;
	
};

