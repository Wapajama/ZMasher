#include "ZMRenderer.h"
#include "Camera.h"
#include "ZMD3DInterface.h"
#include "TextureShaderClass.h"
#include "ZMasherMain.h"
#include "ZMasherUtilities.h"
#include "ZMModelFactory.h"

ZMRenderer::ZMRenderer(void)
{
	m_Camera = nullptr;
}


ZMRenderer::~ZMRenderer(void)
{
}

void ZMRenderer::Render(ZMD3DInterface& d3dinterface)
{
	DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
	m_Camera->UpdateProjMatrix();

	m_Camera->GetWorldOrientation(cameraWorldMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	/*
		TODO:
			- Fix shaders into proper files instead of fucking retarded classes -.-
			- Should probably wait with the implementation of the data orientation
			until... something?
			- Fix model loading for different cool formats, another big project!
			- For .obj, .COLLADA, .mesh , .dae, and see what happens :3
			- Another project to create an in-house filetype, so that we can tailor in- and outputs as we like

	*/

	for (int i = 0; i < m_ModelInstances.size(); ++i)
	{
		m_ModelInstances[i].GetModel()->SetRenderVars(d3dinterface.GetContext());

		ZMasher::Vector3f vPosition = m_ModelInstances[i].GetPosition();
		__m128 posArray;
		posArray.m128_f32[0] = vPosition.x;
		posArray.m128_f32[1] = vPosition.y;
		posArray.m128_f32[2] = vPosition.z;
		posArray.m128_f32[3] = 1.f;

		DirectX::XMVECTOR position(posArray);

		modelWorldMatrix = DirectX::XMMatrixTranslationFromVector(position);

		const bool test = m_TextureShader->SetShaderVars(d3dinterface.GetContext(),
														 modelWorldMatrix,
														 cameraWorldMatrix,
														 projectionMatrix,
														 m_ModelInstances[i].GetModel()->GetTexture());

		assert(test);

		d3dinterface.GetContext()->DrawIndexed(m_ModelInstances[i].GetModel()->GetIndexCount(), 0, 0);

	}


}

void ZMRenderer::Init(ZMD3DInterface& d3dinterface)
{
	ZMModelFactory::Instance()->Create();
	//m_Models.push_back(*ZMModelFactory::Instance()->LoadModel(d3dinterface.GetDevice(), "../Data/cup.obj"));
	m_Models.push_back(*ZMModelFactory::Instance()->LoadFBXModel(d3dinterface.GetDevice(), "../data/birch_bare01.fbx"));

	ZMasher::Vector3f position(0,1, 0.f);

	for (int i = 0; i < 1; ++i)
	{
		ZMModelInstance instance;
		instance.SetModel(&m_Models[0]);//REAL DANGEROUS, CHANGE ASAP
		instance.SetPosition(position);
		position.x -= 3.f;

		m_ModelInstances.push_back(instance);
	}

	m_TextureShader = new TextureShaderClass();

	const bool test2 = m_TextureShader->Init(ZMasherMain::Instance()->GetD3DInterface()->GetDevice(),
											 0);//0 = fuck messages

	assert(test2);
}
