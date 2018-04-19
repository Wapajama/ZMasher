#pragma once

class ID3D11ShaderResourceView;

class Texture
{
public:
	Texture();
	~Texture();

	inline void SetResourceView(ID3D11ShaderResourceView* resource_view) {m_ResourceView = resource_view;}
	inline ID3D11ShaderResourceView* GetResourceView() { return m_ResourceView; }

private:

	ID3D11ShaderResourceView* m_ResourceView;

};

