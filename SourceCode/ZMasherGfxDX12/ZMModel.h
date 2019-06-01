#pragma once

#include <D3dx12.h>
#include <DirectXMath.h>
#include "ZMVertexTypes.h"

typedef VertexPosNormUV CurrentVertexType;//TODO: Repeal and replace!
class Material;
struct ResGUID
{
	char m_Tag[128];
};


class ZMModel
{
public:
	ZMModel();
	~ZMModel();

	void ShutDown();
	void SetRenderVars(ID3D12Device* context);

	int GetIndexCount();

	inline Material* GetMaterial() { return m_Material; };

	bool CreateModel(ID3D12Device* device, CurrentVertexType* vertices, unsigned long* indices, int n_verts, int n_indices, Material* material);

	//TODO: make interface for creating arbitrary shapes
	void CreateCube(ID3D12Device* device, CurrentVertexType*& vertices, unsigned long*& indices, Material* material);
	void CreatePlaneHorizontal(ID3D12Device* device, CurrentVertexType*& vertices, unsigned long*& indices, Material* material,
								const float height = -5.f, 
								const float width = 1000.f,
								const float length = 1000.f);

private:

	bool InitBuffers(ID3D12Device* device, CurrentVertexType* vertices, unsigned long* indices);
	bool Init(ID3D12Device* device, CurrentVertexType* vertices, unsigned long* indices, Material* material);

	void ShutDownBuffers();
	void SetBufferVars(ID3D12Device* context);

	ID3D12Device* m_VertexBuffer;
	ID3D12Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;

	Material* m_Material;

};

