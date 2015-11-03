#include "ModelClass.h"



ModelClass::ModelClass()
{
	m_VertexCount = 3;
	m_IndexCount = 3;

	m_IndexBuffer = nullptr;
	m_VertexBuffer = nullptr;
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Init(ID3D11Device* device)
{
	bool success = false;

	success = InitBuffers(device);
	if (success == false)
	{
		assert(false);
		return false;
	}

	success = LoadTexture( device, L"../../ZMasher/Data/seafloor.dds");

	if (success == false)
	{
		assert(false);
		return false;
	}

	return true;
}
void ModelClass::ShutDown()
{
	ShutDownBuffers();
	if (m_Texture != nullptr)
	{
		m_Texture->Release();
		delete m_Texture;
		m_Texture = nullptr;
	}
}
void ModelClass::SetRenderVars(ID3D11DeviceContext* context)
{
	SetBufferVars(context);
}

int ModelClass::GetIndexCount()
{
	return m_IndexCount;
}

bool ModelClass::InitBuffers(ID3D11Device* device)
{
	CurrentVertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
		
	SinfulHardcoding(vertices, indices);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CurrentVertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}
void ModelClass::ShutDownBuffers()
{
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}
}
void ModelClass::SetBufferVars(ID3D11DeviceContext* context)
{
	unsigned int stride = sizeof(CurrentVertexType);
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* path )
{
	if (m_Texture != nullptr)
	{
		m_Texture->Release();
		delete m_Texture;
	}
	m_Texture = new TextureClass();

	const bool success = m_Texture->Init(device, path);

	return success;
}

void ModelClass::SinfulHardcoding(CurrentVertexType*& vertices, unsigned long*& indices)
{
	m_VertexCount = 36;

	m_IndexCount = 36;

	vertices = new CurrentVertexType[m_VertexCount];

	indices = new unsigned long[m_IndexCount];

	//vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	////vertices[0].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[0].tex = DirectX::XMFLOAT2(0.0f, 1.0f);

	//vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	////vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[1].tex = DirectX::XMFLOAT2(0.5f, 0.0f);

	//vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	////vertices[2].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//vertices[2].tex = DirectX::XMFLOAT2(1.0f, 1.0f);

	//vertices[0] = CurrentVertexType( 0,0,0,0,0 );

	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top middle.
	//indices[2] = 2;  // Bottom right.

	//cube

	int iterate = 0;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0,-1.0, 0.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0,-1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0,-1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0,-1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0,-1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0,-1.0, 1.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0,-1.0, 0.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0, 1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0,-1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0,-1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0, 1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0, 1.0, 1.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0, 1.0, 0.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0, 1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0, 1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0, 1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0, 1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0, 1.0, 1.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0, 1.0, 0.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0,-1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0, 1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0, 1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0,-1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0,-1.0, 1.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0, 1.0, 0.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0, 1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0,-1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 , 1.0,-1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0, 1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 , 1.0,-1.0, 1.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0,-1.0, 0.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0,-1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0, 1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType(-1.0 ,-1.0, 1.0, 0.0, 1.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0,-1.0, 1.0, 0.0 );indices[iterate] = iterate; ++iterate;
	vertices[iterate] = CurrentVertexType( 1.0 ,-1.0, 1.0, 1.0, 1.0 );indices[iterate] = iterate; ++iterate;



}
