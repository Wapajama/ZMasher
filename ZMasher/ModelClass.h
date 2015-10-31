#pragma once
#include <D3D11.h>
#include <DirectXMath.h>

class ModelClass
{
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
		
public:
	ModelClass();
	~ModelClass();

	bool Init(ID3D11Device* device);
	void ShutDown();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount();

private:

	bool InitBuffers(ID3D11Device* device);
	void ShutDownBuffers();
	void RenderBuffers(ID3D11DeviceContext* context);

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;

};

