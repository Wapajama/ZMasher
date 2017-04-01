#pragma once

/*
	TODO: forward declare DX11 includes
*/

#include <d3d11.h>


class TextureClass
{
public:
	TextureClass(void);
	TextureClass(const TextureClass&);
	~TextureClass(void);

	bool Init(ID3D11Device* device, const char* fileName);
	void Release();

	ID3D11ShaderResourceView* GetTexture();

private:

	ID3D11ShaderResourceView* m_Texture;

};

