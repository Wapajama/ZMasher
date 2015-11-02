#include "TextureClass.h"
#include <D3DX11.h>

TextureClass::TextureClass(void)
{
	m_Texture = nullptr;
}


TextureClass::~TextureClass(void)
{
}

bool TextureClass::Init(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT hr = S_OK;

	hr = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_Texture, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void TextureClass::Release()
{
	if (m_Texture != nullptr)
	{
		m_Texture->Release();
		m_Texture = nullptr;
	}
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_Texture;
}
