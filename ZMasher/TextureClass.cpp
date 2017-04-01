#include "TextureClass.h"
#include <D3DX11.h>
#include <string>
TextureClass::TextureClass(void)
{
	m_Texture = nullptr;
}


TextureClass::~TextureClass(void)
{
}

bool TextureClass::Init(ID3D11Device* device, const char* fileName)
{
	HRESULT hr = S_OK;
	std::string filename_small = fileName;
	std::wstring filename_wide(filename_small.begin(), filename_small.end());

	hr = D3DX11CreateShaderResourceViewFromFile(device, filename_wide.c_str(), NULL, NULL, &m_Texture, NULL);
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
