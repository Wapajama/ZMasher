#include "ZMRenderer.h"
#include "Camera.h"
#include "ZMD3DInterface.h"

#include "ModelShader.h"
#include <ZMUtils\Utility\ZMasherUtilities.h>
#include "ZMModelFactory.h"
#include "ZMModelNode.h"

#include <D3D11.h>

void SetXMMatrix(DirectX::XMMATRIX& matrix, const ZMasher::Matrix44f& other)
{
	matrix.r[0].m128_f32[0] = other.m_Elements[0][0];
	matrix.r[0].m128_f32[1] = other.m_Elements[0][1];
	matrix.r[0].m128_f32[2] = other.m_Elements[0][2];
	matrix.r[0].m128_f32[3] = other.m_Elements[0][3];
							  
	matrix.r[1].m128_f32[0] = other.m_Elements[1][0];
	matrix.r[1].m128_f32[1] = other.m_Elements[1][1];
	matrix.r[1].m128_f32[2] = other.m_Elements[1][2];
	matrix.r[1].m128_f32[3] = other.m_Elements[1][3];
							  
	matrix.r[2].m128_f32[0] = other.m_Elements[2][0];
	matrix.r[2].m128_f32[1] = other.m_Elements[2][1];
	matrix.r[2].m128_f32[2] = other.m_Elements[2][2];
	matrix.r[2].m128_f32[3] = other.m_Elements[2][3];
							  
	matrix.r[3].m128_f32[0] = other.m_Elements[3][0];
	matrix.r[3].m128_f32[1] = other.m_Elements[3][1];
	matrix.r[3].m128_f32[2] = other.m_Elements[3][2];
	matrix.r[3].m128_f32[3] = other.m_Elements[3][3];
}

ZMRenderer::ZMRenderer(void)
{
	m_Camera = nullptr;
}

ZMRenderer::~ZMRenderer(void)
{
	ZMModelFactory::Instance()->Release();
}

void ZMRenderer::Render(ZMD3DInterface& d3dinterface, const float dt)
{
	const float dt_cap = FLT_MAX;
	m_Dt+= dt;
	if (m_Dt > dt_cap)
	{
		m_Dt-=dt_cap;
	}
	
#ifdef BENCHMARK
	Profiler::Instance()->BeginTask(m_ModelsTimeStamp);
#endif // BENCHMARK

	for (short i = ZMModelFactory::Instance()->m_ModelInstances.Size()-1; i >= 0; --i)
	{
		if (ZMModelFactory::Instance()->m_ModelInstances[i]->IsMarkedForDelete())
		{
			ZMModelFactory::Instance()->m_ModelInstances.RemoveCyclic(i);//hope to fuck none of you squired ass n****z aint gon and hidin some danglin pointers in da hood yao -.-
		}
	}
	RenderSkybox(d3dinterface);
	Render2DTerrain(d3dinterface);
	for (int i = 0; i < ZMModelFactory::Instance()->m_ModelInstances.Size(); ++i)
	{
		RenderModelHierarchy(d3dinterface, ZMModelFactory::Instance()->m_ModelInstances[i], ZMasher::Matrix44f::Identity());
	}
#ifdef BENCHMARK
	Profiler::Instance()->EndTask(m_ModelsTimeStamp);
#endif // BENCHMARK
}

void ZMRenderer::Init(ZMD3DInterface& d3dinterface, Profiler* profiler, TimerManager* instance)
{
	Profiler::Create(profiler);
	TimerManager::Create(instance);

#ifdef BENCHMARK
	m_ModelsTimeStamp = Profiler::Instance()->AddTask("RenderModels");
#endif // BENCHMARK

	ZMModelFactory::Instance()->Create();
	ZMModelFactory::Instance()->SetDevice(d3dinterface.GetDevice());

	m_Shader = new ModelShader();

	const bool succeded = m_Shader->Create(L"PBRShader.fx", d3dinterface.GetDevice());
	ASSERT(succeded, "shader failed to init!");

	m_SkyboxShader = new ModelShader();
	const bool succeded_skybox = m_SkyboxShader->Create(L"Skybox.fx", d3dinterface.GetDevice());
	ASSERT(succeded_skybox, "shader failed to init!");

	m_Skybox = ZMModelFactory::Instance()->LoadSkyBox("../../data/cubemaps/Skybox001.dds");

	m_TerrainShader = new ModelShader();
	const bool succeded_flatTerrain = m_TerrainShader->Create(L"FlatTerrain.fx", d3dinterface.GetDevice());
	ASSERT(succeded_flatTerrain, "shader failed to init!");

	m_Terrain = ZMModelFactory::Instance()->Load2DTerrain("../../data/maps/grass.dds");

}

void ZMRenderer::RenderGrid(ZMD3DInterface& d3dinterface)
{
	
}

//BOILERPLATE 
//BOILERPLATE 
//EVERYWHERE :D:D:D:D
//(but seriously extract the everloving shit outta this)

void ZMRenderer::RenderModelHierarchy(ZMD3DInterface& d3dinterface, ZMModelInstanceNode* model, const ZMasher::Matrix44f& parent_orientation)
{
	if (model->GetModelNode()->GetModel() != nullptr)//TODO: wtf is the model nullptr? -.-
	{
		DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
		m_Camera->UpdateProjMatrix();

		const ZMasher::Matrix44f view_matrix = ~m_Camera->GetWorldOrientation();
		cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
		m_Camera->GetProjectionMatrix(projectionMatrix);

		model->GetModelNode()->GetModel()->SetRenderVars(d3dinterface.GetContext());//TODO: replace this with lazy update

		const ZMasher::Matrix44f current_transform =  model->GetTransform()* parent_orientation;

		//modelWorldMatrix.r[0] = current_transform.m_Data[0];
		//modelWorldMatrix.r[1] = current_transform.m_Data[1];
		//modelWorldMatrix.r[2] = current_transform.m_Data[2];
		//modelWorldMatrix.r[3] = current_transform.m_Data[3];
		SetXMMatrix(modelWorldMatrix, current_transform);

		DirectX::XMVECTOR cam_pos;// = m_Camera->GetPosition().m_Data;
		cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
		cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
		cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
		cam_pos.m128_f32[3] = 1.f;
		const bool succeded = m_Shader->SetShaderVars(d3dinterface.GetContext(),
														{modelWorldMatrix,
														cameraWorldMatrix,
														projectionMatrix,
														cam_pos, 
														m_Dt});
		ASSERT(succeded, "shader failed to init!");
		ModelShader* model_shader = reinterpret_cast<ModelShader*>(m_Shader);
		Material* material = model->GetModelNode()->GetModel()->GetMaterial();

		model_shader->SetShaderResource(eTextureType::ALBEDO, material->GetTexture(eTextureType::ALBEDO));
		model_shader->SetShaderResource(eTextureType::NORMAL, material->GetTexture(eTextureType::NORMAL));
		model_shader->SetShaderResource(eTextureType::AMBIENT_OCCLUSION, material->GetTexture(eTextureType::AMBIENT_OCCLUSION));
		model_shader->SetShaderResource(eTextureType::ROUGHNESS, material->GetTexture(eTextureType::ROUGHNESS));
		model_shader->SetShaderResource(eTextureType::SUBSTANCE, material->GetTexture(eTextureType::SUBSTANCE));
		
		m_Shader->Apply(d3dinterface.GetContext());

		d3dinterface.GetContext()->DrawIndexed(model->GetModelNode()->GetModel()->GetIndexCount(), 0, 0);
		
	}
	for (short i = 0; i < model->ChildCount(); ++i)
	{
		RenderModelHierarchy(d3dinterface, model->GetChild(i), model->GetTransform());
	}
}

void ZMRenderer::RenderSkybox(ZMD3DInterface& d3dinterface)
{
	DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
	m_Camera->UpdateProjMatrix();
	ZMasher::Matrix44f camera_ori;
	//camera_ori = m_Camera->GetWorldOrientation();
	m_Camera->GetWorldOrientationaTest(camera_ori);
	camera_ori.SetTranslation(ZMasher::Vector4f(0,0,0,1));
	const ZMasher::Matrix44f view_matrix = ~camera_ori;
	cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	m_Skybox->GetModelNode()->GetModel()->SetRenderVars(d3dinterface.GetContext());//TODO: replace this with lazy update

	if (m_Skybox == nullptr)
	{
		return;
	}
	ZMasher::Matrix44f current_transform; 
	current_transform = m_Skybox->GetTransform();

	//modelWorldMatrix.r[0] = current_transform.m_Data[0];
	//modelWorldMatrix.r[1] = current_transform.m_Data[1];
	//modelWorldMatrix.r[2] = current_transform.m_Data[2];
	//modelWorldMatrix.r[3] = current_transform.m_Data[3];
	SetXMMatrix(modelWorldMatrix, current_transform);

	if (m_Camera != nullptr)
	{
		DirectX::XMVECTOR cam_pos;// = m_Camera->GetPosition().m_Data;
		cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
		cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
		cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
		cam_pos.m128_f32[3] = 1.f;
		const bool succeded = m_SkyboxShader->SetShaderVars(d3dinterface.GetContext(),
		{ modelWorldMatrix,
		cameraWorldMatrix,
		projectionMatrix,
		cam_pos,
		m_Dt });
		ASSERT(succeded, "shader failed to init!");
	}
	ModelShader* model_shader = reinterpret_cast<ModelShader*>(m_SkyboxShader);
	Material* material = m_Skybox->GetModelNode()->GetModel()->GetMaterial();

	model_shader->SetShaderResource(eTextureType::ALBEDO, material->GetTexture(eTextureType::ALBEDO));
		
	m_SkyboxShader->Apply(d3dinterface.GetContext());

	d3dinterface.GetContext()->DrawIndexed(m_Skybox->GetModelNode()->GetModel()->GetIndexCount(), 0, 0);
}

void ZMRenderer::Render2DTerrain(ZMD3DInterface& d3dinterface)
{
	DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
	m_Camera->UpdateProjMatrix();
	ZMasher::Matrix44f camera_ori;// = m_Camera->GetWorldOrientation();
	m_Camera->GetWorldOrientationaTest(camera_ori);
	const ZMasher::Matrix44f view_matrix = ~camera_ori;
	cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	m_Terrain->GetModelNode()->GetModel()->SetRenderVars(d3dinterface.GetContext());//TODO: replace this with lazy update

	ZMasher::Matrix44f current_transform;
	current_transform = m_Terrain->GetTransform();

	//modelWorldMatrix.r[0] = current_transform.m_Data[0];
	//modelWorldMatrix.r[1] = current_transform.m_Data[1];
	//modelWorldMatrix.r[2] = current_transform.m_Data[2];
	//modelWorldMatrix.r[3] = current_transform.m_Data[3];
	SetXMMatrix(modelWorldMatrix, current_transform);

	DirectX::XMVECTOR cam_pos;// = m_Camera->GetPosition().m_Data;
	cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
	cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
	cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
	cam_pos.m128_f32[3] = 1.f;

	const bool succeded = m_TerrainShader->SetShaderVars(d3dinterface.GetContext(),
														{modelWorldMatrix,
														cameraWorldMatrix,
														projectionMatrix,
														cam_pos, 
														m_Dt});
	ASSERT(succeded, "shader failed to init!");
	ModelShader* model_shader = reinterpret_cast<ModelShader*>(m_TerrainShader);
	Material* material = m_Terrain->GetModelNode()->GetModel()->GetMaterial();

	model_shader->SetShaderResource(eTextureType::ALBEDO, material->GetTexture(eTextureType::ALBEDO));
		
	m_TerrainShader->Apply(d3dinterface.GetContext());

	d3dinterface.GetContext()->DrawIndexed(m_Terrain->GetModelNode()->GetModel()->GetIndexCount(), 0, 0);
}