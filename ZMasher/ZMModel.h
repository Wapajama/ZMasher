#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include "ZMVertexTypes.h"

typedef VertexPosNormUV CurrentVertexType;
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
	void SetRenderVars(ID3D11DeviceContext* context);

	int GetIndexCount();

	inline Material* GetMaterial() { return m_Material; };

	//TODO: move the textures someplace else
	bool CreateModel(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices, int n_verts, int n_indices, Material* material);

private:

	bool InitBuffers(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices);
	bool Init(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices, Material* material);

	void ShutDownBuffers();
	void SetBufferVars(ID3D11DeviceContext* context);

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;

	Material* m_Material;

};

