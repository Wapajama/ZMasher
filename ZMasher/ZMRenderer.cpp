#include "ZMRenderer.h"
#include "CameraClass.h"
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
	m_Camera->UpdateViewMatrix();
	
	worldMatrix = DirectX::XMMatrixIdentity();
	
	m_Camera->GetViewMatrix(viewMatrix);
	d3dinterface.GetProjectionMatrix(projectionMatrix);

	for (int i = 0; i < m_Models.size(); ++i)
	{
		m_Models[i].SetRenderVars(d3dinterface.GetContext());

		//const bool test = m_Shader->SetShaderVars(m_D3DInterface.GetContext(),
		//				 worldMatrix,
		//				 viewMatrix,
		//				 projectionMatrix);

		const bool test = m_TextureShader->SetShaderVars(d3dinterface.GetContext(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_Models[i].GetTexture());
			
		assert(test);

		d3dinterface.GetContext()->DrawIndexed(m_Models[i].GetIndexCount(), 0, 0); 
		
	}
	
	
}

void ZMRenderer::Init()
{
	//m_Model = new ZMModel();
	//m_Model->Init(m_D3DInterface.GetDevice());

	ZMModel model;
	model.Init(ZMasherMain::Instance()->GetD3DInterface()->GetDevice());
	m_Models.push_back(model);

	//m_Shader = new ColorClassShader();
	m_TextureShader = new TextureShaderClass();

	//const bool test2 = m_Shader->Init(	m_D3DInterface.GetDevice(),
	//									m_WinVals.m_WindowHandle);

	const bool test2 = m_TextureShader->Init(ZMasherMain::Instance()->GetD3DInterface()->GetDevice(),
											 0);//0 = fuck messages

	assert(test2);
}
