#pragma once

#include <DataStructures\GrowArray.h>
#include <ZMasherGfxDX11\Texture.h>
#include <string>
class ID3D11Device;
class TextureContainer
{
public:
	TextureContainer();
	~TextureContainer();

	Texture* GetTexture(const char* file_path);
	inline void SetDevice(ID3D11Device* device){ m_Device = device;};

private:

	struct TexturePathPair
	{
		std::string m_Path;
		Texture m_Texture;
	};
	GrowArray<TexturePathPair> m_Textures;
	ID3D11Device* m_Device;

	Texture* LoadTexture(const char* file_path);
	 
};

