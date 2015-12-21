#include "ZMRenderer.h"
#include "Camera.h"
#include "ZMD3DInterface.h"
#include "TextureShaderClass.h"
#include "ZMasherMain.h"
#include "ZMasherUtilities.h"

ZMRenderer::ZMRenderer(void)
{
	m_Camera = nullptr;
}


ZMRenderer::~ZMRenderer(void)
{
}

void ZMRenderer::Render(ZMD3DInterface& d3dinterface)
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Camera->UpdateProjMatrix();
	
	worldMatrix = DirectX::XMMatrixIdentity();
	
	m_Camera->GetWorldOrientation(viewMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);

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

		worldMatrix = DirectX::XMMatrixTranslationFromVector(position);

		const bool test = m_TextureShader->SetShaderVars(d3dinterface.GetContext(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_ModelInstances[i].GetModel()->GetTexture());
			
		assert(test);

		d3dinterface.GetContext()->DrawIndexed(m_ModelInstances[i].GetModel()->GetIndexCount(), 0, 0); 
		
	}
	
	
}

void ZMRenderer::Init()
{
	ZMModel model;
	model.Init(ZMasherMain::Instance()->GetD3DInterface()->GetDevice());
	m_Models.push_back(model);

	ZMasher::Vector3f position(5.f * 3.f,0,0.f);

	for (int i = 0; i < 10; ++i)
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