#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <Math/ZMVector.h>

#define Float4InputDesc DXGI_FORMAT_R32G32B32A32_FLOAT
#define Float2InputDesc DXGI_FORMAT_R32G32_FLOAT

static D3D11_INPUT_ELEMENT_DESC g_PosUv[2]
{
	{ "POSITION", 0, Float4InputDesc,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, Float2InputDesc,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

static D3D11_INPUT_ELEMENT_DESC g_PosNormUv[3]
{
	{ "POSITION", 0, Float4InputDesc,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, Float2InputDesc,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0,	 Float4InputDesc,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
					m_Position.x = x;
					m_Position.y = y;
					m_Position.z = z;

					m_Tex.x = u;
					m_Tex.y = v;
				}
				

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT2 m_Tex;
};

struct VertexPosNormUV
{
	VertexPosNormUV() {}

	VertexPosNormUV(const float x,
					const float y,
					const float z,
					const float u,
					const float v)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
		m_Position.w = 0;

		m_Tex.x = u;
		m_Tex.y = v;

		m_Normal.x = 0;
		m_Normal.y = 0;
		m_Normal.z = 0;
		m_Normal.w = 0;
	}


	DirectX::XMFLOAT4 m_Position;
	DirectX::XMFLOAT2 m_Tex;
	DirectX::XMFLOAT4 m_Normal;
};

struct VertexPosCol
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};
