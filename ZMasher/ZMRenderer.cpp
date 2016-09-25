#include "ZMRenderer.h"
#include "Camera.h"
#include "ZMD3DInterface.h"
#include "TextureShaderClass.h"
#include "ZMasherMain.h"
#include "ZMasherUtilities.h"
#include "ZMModelFactory.h"
#include "ZMModelNode.h"

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
		ZMasher::Matrix44f transform = m_ModelInstances[i]->GetTransform();
		m_ModelInstances[i]->SetTransform(ZMasher::Matrix44f::CreateRotationMatrixY(0.016f) * transform);
		RenderModelHierarchy(d3dinterface, m_ModelInstances[i], ZMasher::Matrix44f::Identity());
	}
}

void ZMRenderer::Init(ZMD3DInterface& d3dinterface)
{
	ZMModelFactory::Instance()->Create();

	ZMasher::Vector3f position(0, 1, 0.f);

	for (int i = 0; i < 1; ++i)
	{
		m_ModelInstances.push_back(ZMModelFactory::Instance()->LoadModelInstance(d3dinterface.GetDevice(), "../data/Truax_Studio_Mac11.FBX"));
	}

	m_TextureShader = new TextureShaderClass();

	const bool succeded = m_TextureShader->Init(ZMasherMain::Instance()->GetD3DInterface()->GetDevice(),
											 0);//0 = fuck messages

	ASSERT(succeded, "shader failed to init!");
}

void ZMRenderer::RenderGrid(ZMD3DInterface& d3dinterface)
{
	
}

void ZMRenderer::RenderModelHierarchy(ZMD3DInterface& d3dinterface, ZMModelInstanceNode* model, const ZMasher::Matrix44f& parent_orientation)
{
	if (model->GetModel() != nullptr)
	{
		DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
		m_Camera->UpdateProjMatrix();
		m_Camera->GetWorldOrientation(cameraWorldMatrix);
		m_Camera->GetProjectionMatrix(projectionMatrix);

		model->GetModel()->SetRenderVars(d3dinterface.GetContext());


		const ZMasher::Matrix44f current_transform =  parent_orientation * model->GetTransform();

		modelWorldMatrix.r[0] = current_transform.m_Data[0];
		modelWorldMatrix.r[1] = current_transform.m_Data[1];
		modelWorldMatrix.r[2] = current_transform.m_Data[2];
		modelWorldMatrix.r[3] = current_transform.m_Data[3];

		const bool succeded = m_TextureShader->SetShaderVars(d3dinterface.GetContext(),
														 modelWorldMatrix,
														 cameraWorldMatrix,
														 projectionMatrix,
														 model->GetModel()->GetTexture());

		ASSERT(succeded, "shader failed to init!");

		d3dinterface.GetContext()->DrawIndexed(model->GetModel()->GetIndexCount(), 0, 0);
	}
	for (short i = 0; i < model->ChildCount(); ++i)
	{
		RenderModelHierarchy(d3dinterface, model->GetChild(i), model->GetTransform());
	}
}
