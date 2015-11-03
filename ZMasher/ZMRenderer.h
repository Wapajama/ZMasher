#pragma once
#include <vector> // TODO: create an in-house data structure for managing models
#include "ZMModel.h"
#include "ZMModelInstance.h"

class CameraClass;
class ZMD3DInterface;
class TextureShaderClass;

class ZMRenderer
{
public:
	ZMRenderer(void);
	~ZMRenderer(void);

	
	void Render(ZMD3DInterface& d3dinterface);

	void Init();

	__forceinline void SetCamera(CameraClass* camera);

private:
	
	TextureShaderClass* m_TextureShader;//TODO: create a neat system for shaders
	CameraClass* m_Camera;
	std::vector<ZMModel> m_Models;
	std::vector<ZMModelInstance> m_ModelInstances;
};

__forceinline void ZMRenderer::SetCamera(CameraClass* camera)
{
	m_Camera = camera;
}