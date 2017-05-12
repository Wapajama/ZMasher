#include "TextureContainer.h"
#include <D3DX11.h>

TextureContainer::TextureContainer()
	: m_Device(nullptr)
{
}


TextureContainer::~TextureContainer()
{
}

Texture* TextureContainer::GetTexture(const char* file_path)
{
	for (short i = 0; i < m_Textures.Size(); ++i)
	{
		if (std::strcmp(m_Textures[i].m_Path.c_str(), file_path) == 0)
		{
			return &m_Textures[i].m_Texture;
		}
	}
	return LoadTexture(file_path);
}

Texture* TextureContainer::LoadTexture(const char* file_path)
{
	TexturePathPair texture_pair;

	HRESULT hr = S_OK;
	std::string filename_small = file_path;
	std::wstring filename_wide(filename_small.begin(), filename_small.end());
	ID3D11ShaderResourceView* resource_view = NULL;
	hr = D3DX11CreateShaderResourceViewFromFile(m_Device, filename_wide.c_str(), NULL, NULL, &resource_view, NULL);
	if (FAILED(hr))
	{
		ASSERT(false, "Couldn't load texture!");
		return nullptr;
	}
	texture_pair.m_Texture.SetResourceView(resource_view);
	m_Textures.Add(texture_pair);

	return &m_Textures.GetLast().m_Texture;
}