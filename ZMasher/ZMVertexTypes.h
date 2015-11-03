#pragma once
#include <DirectXMath.h>

struct VertexPosUV
{
	VertexPosUV(){}

	VertexPosUV(const float x,
				const float y,
				const float z,
				const float u,
				const float v)
				{
					position.x = x;
					position.y = y;
					position.z = z;

					tex.x = u;
					tex.y = v;
				}
				

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 tex;		
};

struct VertexPosCol
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};