#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include "ZMVertexTypes.h"

typedef VertexPosNormUV CurrentVertexType;

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

	ID3D11ShaderResourceView* GetTexture();

	//TODO: move the textures someplace else
	bool CreateModel(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices, int n_verts, int n_indices, const char* texture_path);

private:

	bool InitBuffers(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices);
	bool Init(ID3D11Device* device, CurrentVertexType* vertices, unsigned long* indices, const char* texture_path);

	void ShutDownBuffers();
	void SetBufferVars(ID3D11DeviceContext* context);

	bool LoadTexture(ID3D11Device* device, const char* path);
	void ReleaseTexture();

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;

	TextureClass* m_Texture;

};

