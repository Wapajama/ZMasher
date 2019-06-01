#pragma once

#include <DataStructures\GrowArray.h>
#include <ZMasherGfxDX12\Texture.h>
#include <string>
class ID3D12Device;
class TextureContainer
{
public:
	TextureContainer();
	~TextureContainer();

	Texture* GetTexture(const char* file_path);
	inline void SetDevice(ID3D12Device* device){ m_Device = device;};

private:

	struct TexturePathPair
	{
		std::string m_Path;
		Texture m_Texture;
	};
	GrowArray<TexturePathPair> m_Textures;
	ID3D12Device* m_Device;

	Texture* LoadTexture(const char* file_path);
	 
};

