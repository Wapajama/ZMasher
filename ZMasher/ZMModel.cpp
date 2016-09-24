#include "ZMModel.h"
#include <Debugging\ZMDebugger.h>


ZMModel::ZMModel()
{
	m_VertexCount = 3;
	m_IndexCount = 3;

	m_IndexBuffer = nullptr;
	m_VertexBuffer = nullptr;
}


ZMModel::~ZMModel()
{
}

bool ZMModel::Init(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices)
{
	bool success = false;

	success = InitBuffers(device, vertices, indices);
	if (success == false)
	{
		ASSERT(false, "Failed to init model!");
		return false;
	}

	success = LoadTexture( device, L"../../ZMasher/Data/mactex.jpg");

	if (success == false)
	{
		assert(false);
		return false;
	}

	return true;
}
void ZMModel::ShutDown()
{
	ShutDownBuffers();
	if (m_Texture != nullptr)
	{
		m_Texture->Release();
		delete m_Texture;
		m_Texture = nullptr;
	}
}
void ZMModel::SetRenderVars(ID3D11DeviceContext* context)
{
	SetBufferVars(context);
}

int ZMModel::GetIndexCount()
{
	return m_IndexCount;
}

bool ZMModel::InitBuffers(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices)
{
	//CurrentVertexType* vertices = nullptr;
	//unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
		
	//SinfulHardcoding(vertices, indices);

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
void ZMModel::ShutDownBuffers()
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
void ZMModel::SetBufferVars(ID3D11DeviceContext* context)
{
	unsigned int stride = sizeof(CurrentVertexType);
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11ShaderResourceView* ZMModel::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ZMModel::LoadTexture(ID3D11Device* device, WCHAR* path )
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

void ZMModel::SinfulHardcoding(CurrentVertexType*& vertices, unsigned long*& indices)
{
	m_VertexCount = 36;
	m_IndexCount = 36;

	vertices = new CurrentVertexType[m_VertexCount];

	indices = new unsigned long[m_IndexCount];

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

bool ZMModel::CreateModel(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices, int n_verts, int n_indices)
{
	m_VertexCount = n_verts;
	m_IndexCount = n_indices;
	return Init(device, vertices, indices);
}