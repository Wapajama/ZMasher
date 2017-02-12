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
	void SetOrientation(const ZMasher::Matrix44f& orientation);

	void UpdateProjMatrix();
	void GetOrientation(DirectX::XMMATRIX& matrix);
	void GetProjectionMatrix(DirectX::XMMATRIX& matrix);

	ZMasher::Vector3f GetPosition();
	ZMasher::Matrix44f GetWorldOrientation()const;

	void SetWindowSize(const ZMasher::Vector2i& size);
	const ZMasher::Vector2i GetWindowSize()const;

	void RotateX(const float radians);
	void RotateY(const float radians);
	void RotateZ(const float radians);

private:

	float m_FOV;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

	float m_XRotation;
	float m_YRotation;

	ZMasher::Vector2<int> m_WindowSize;

	ZMasher::Matrix44f m_Orientation;
	ZMasher::Matrix44f m_ProjectionMatrix;
	
};

