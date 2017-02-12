#include "ZMRenderer.h"
#include "Camera.h"
#include "ZMD3DInterface.h"

#include "ModelShader.h"
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
	/*
		TODO:
			- Fix shaders into proper files instead of fucking retarded classes -.-
			- Should probably wait with the implementation of the data orientation
			until... something?
			- Fix model loading for different cool formats, another big project!
			- For .obj, .COLLADA, .mesh , .dae, and see what happens :3
			- Another project to create an in-house filetype, so that we can tailor in- and outputs as we like
	*/
	for (int i = 0; i < ZMModelFactory::Instance()->m_ModelInstances.Size(); ++i)
	{
		RenderModelHierarchy(d3dinterface, ZMModelFactory::Instance()->m_ModelInstances[i], ZMasher::Matrix44f::Identity());
	}
}

void ZMRenderer::Init(ZMD3DInterface& d3dinterface)
{
	ZMModelFactory::Instance()->Create();
	ZMModelFactory::Instance()->SetDevice(d3dinterface.GetDevice());

	m_Shader = new ModelShader();

	const bool succeded = m_Shader->Create(L"texture.fx", ZMasherMain::Instance()->GetD3DInterface()->GetDevice());
	ASSERT(succeded, "shader failed to init!");
}

void ZMRenderer::RenderGrid(ZMD3DInterface& d3dinterface)
{
	
}

void ZMRenderer::RenderModelHierarchy(ZMD3DInterface& d3dinterface, ZMModelInstanceNode* model, const ZMasher::Matrix44f& parent_orientation)
{
	if (model->GetModelNode()->GetModel() != nullptr)//TODO: wtf is the model nullptr? -.-
	{
		DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
		m_Camera->UpdateProjMatrix();

		const ZMasher::Matrix44f view_matrix = ~m_Camera->GetWorldOrientation();
		cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
		//m_Camera->GetOrientation(cameraWorldMatrix);
		m_Camera->GetProjectionMatrix(projectionMatrix);

		model->GetModelNode()->GetModel()->SetRenderVars(d3dinterface.GetContext());//TODO: replace this with lazy update

		const ZMasher::Matrix44f current_transform =  model->GetTransform()* parent_orientation;

		modelWorldMatrix.r[0] = current_transform.m_Data[0];
		modelWorldMatrix.r[1] = current_transform.m_Data[1];
		modelWorldMatrix.r[2] = current_transform.m_Data[2];
		modelWorldMatrix.r[3] = current_transform.m_Data[3];

		const bool succeded = m_Shader->SetShaderVars(d3dinterface.GetContext(),
														 modelWorldMatrix,
														 cameraWorldMatrix,
														 projectionMatrix);
		ASSERT(succeded, "shader failed to init!");
		reinterpret_cast<ModelShader*>(m_Shader)->SetShaderResource(model->GetModelNode()->GetModel()->GetTexture());
		
		m_Shader->Apply(d3dinterface.GetContext());

		d3dinterface.GetContext()->DrawIndexed(model->GetModelNode()->GetModel()->GetIndexCount(), 0, 0);
	}
	for (short i = 0; i < model->ChildCount(); ++i)
	{
		RenderModelHierarchy(d3dinterface, model->GetChild(i), model->GetTransform());
	}
}
