#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

static D3D11_INPUT_ELEMENT_DESC g_PosUv[2]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

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


