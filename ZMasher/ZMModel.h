#pragma once
#include <D3D11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include "ZMVertexTypes.h"

typedef VertexPosUV CurrentVertexType;

class ZMModel
{
public:
	ZMModel();
	~ZMModel();

	bool Init(ID3D11Device* device);
	void ShutDown();
	void SetRenderVars(ID3D11DeviceContext* context);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:

	bool InitBuffers(ID3D11Device* device);

	void SinfulHardcoding(CurrentVertexType*& vertexes, unsigned long*& indexes);
	void ShutDownBuffers();
	void SetBufferVars(ID3D11DeviceContext* context);

	bool LoadTexture(ID3D11Device* device, WCHAR* path);
	void ReleaseTexture();

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	int m_VertexCount;
	int m_IndexCount;

	TextureClass* m_Texture;

};

