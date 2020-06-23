#include "ZMRenderer.h"
#include "Camera.h"
#include "ZMD3DInterface.h"

#include "ModelShader.h"
#include "DebugLineShader.h"
#include <ZMUtils\Utility\ZMasherUtilities.h>
#include "ZMModelFactory.h"
#include "ZMModelNode.h"
#include <ZMUtils\File\PathManager.h>
#include <D3D11.h>
#include <ZMasherGfxDX11/ZMVertexTypes.h>
#include <ZMasherGfxDX11/DebugLine.h>
#include <Time/Profiler.h>

//void SetXMMatrix(DirectX::XMMATRIX& matrix, const ZMasher::Matrix44f& other)
//{
//	matrix.r[0].m128_f32[0] = other.m_Elements[0][0];
//	matrix.r[0].m128_f32[1] = other.m_Elements[0][1];
//	matrix.r[0].m128_f32[2] = other.m_Elements[0][2];
//	matrix.r[0].m128_f32[3] = other.m_Elements[0][3];
//							  
//	matrix.r[1].m128_f32[0] = other.m_Elements[1][0];
//	matrix.r[1].m128_f32[1] = other.m_Elements[1][1];
//	matrix.r[1].m128_f32[2] = other.m_Elements[1][2];
//	matrix.r[1].m128_f32[3] = other.m_Elements[1][3];
//							  
//	matrix.r[2].m128_f32[0] = other.m_Elements[2][0];
//	matrix.r[2].m128_f32[1] = other.m_Elements[2][1];
//	matrix.r[2].m128_f32[2] = other.m_Elements[2][2];
//	matrix.r[2].m128_f32[3] = other.m_Elements[2][3];
//							  
//	matrix.r[3].m128_f32[0] = other.m_Elements[3][0];
//	matrix.r[3].m128_f32[1] = other.m_Elements[3][1];
//	matrix.r[3].m128_f32[2] = other.m_Elements[3][2];
//	matrix.r[3].m128_f32[3] = other.m_Elements[3][3];
//}

ZMRenderer::ZMRenderer(void)
{
	m_Camera = nullptr;
}

ZMRenderer::~ZMRenderer(void)
{
	ZMModelFactory::Instance()->Release();
}

void ZMRenderer::Destroy()
{
#ifdef BENCHMARK
	 Profiler::Instance()->AddTimeStamp(m_ModelsTimeStamp, "RenderModels");
#endif // BENCHMARK
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
	m_ModelsTimeStamp.Start();
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
	RenderDebugLines(d3dinterface);
	for (int i = 0; i < ZMModelFactory::Instance()->m_ModelInstances.Size(); ++i)
	{
		RenderModelHierarchy(d3dinterface, ZMModelFactory::Instance()->m_ModelInstances[i], ZMasher::Matrix44f::Identity());
	}
#ifdef BENCHMARK
	m_ModelsTimeStamp.EndTimeStamp();
#endif // BENCHMARK
}

void ZMRenderer::Init(ZMD3DInterface& d3dinterface, Profiler* profiler, TimerManager* instance, PathManager* path_instance)
{
	Profiler::Create(profiler);
	TimerManager::Create(instance);
	PathManager::Create(path_instance);

	ZMModelFactory::Instance()->Create();
	ZMModelFactory::Instance()->SetDevice(d3dinterface.GetDevice());
	m_Shader = new ModelShader();
	m_Shader->SetVertexType(&g_PosUVNorm);
	const bool succeeded = m_Shader->Create("PBRShader.fx", d3dinterface.GetDevice());
	ASSERT(succeeded, "model shader failed to init!");
	m_SkyboxShader = new ModelShader();
	m_SkyboxShader->SetVertexType(&g_PosUVNorm);
	const bool succeeded_skybox = m_SkyboxShader->Create("Skybox.fx", d3dinterface.GetDevice());
	ASSERT(succeeded_skybox, "skybox shader failed to init!");
	m_Skybox = ZMModelFactory::Instance()->LoadSkyBox((PathManager::Instance()->GetDataPath() + "cubemaps/Skybox001.dds").c_str());

	m_TerrainShader = new ModelShader();
	m_TerrainShader->SetVertexType(&g_PosUVNorm);
	const bool succeeded_flatTerrain = m_TerrainShader->Create("FlatTerrain.fx", d3dinterface.GetDevice());
	ASSERT(succeeded_flatTerrain, "terrain shader failed to init!");
	m_DebugLineShader = new DebugLineShader();
	m_DebugLineShader->SetVertexType(&g_PosCol);
	const bool succeeded_debugLine = m_DebugLineShader->Create("Color.fx", d3dinterface.GetDevice());
	ASSERT(succeeded_debugLine, "debugLine shader failed to init!");
	m_Terrain = ZMModelFactory::Instance()->Load2DTerrain((PathManager::Instance()->GetDataPath()+ "maps/grass.dds").c_str());

	m_DebugLine = new DebugLine();
	m_DebugLine->Create(d3dinterface.GetDevice());
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
		ZMasher::Matrix44f world = m_Camera->GetWorldOrientation();
		world.SetTranslation(ZMasher::Vector4f(m_Camera->GetPosition(),1));
		const ZMasher::Matrix44f view_matrix = ~world;
		cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
		m_Camera->GetProjectionMatrix(projectionMatrix);

		model->GetModelNode()->GetModel()->SetRenderVars(d3dinterface.GetContext());//TODO: replace this with lazy update

		const ZMasher::Matrix44f current_transform =  model->GetTransform()* parent_orientation;

		current_transform.Get(modelWorldMatrix);

		DirectX::XMVECTOR cam_pos;
#ifndef _XM_NO_INTRINSICS_
		cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
		cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
		cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
		cam_pos.m128_f32[3] = 1.f;
#else
		cam_pos.vector4_f32[0] = m_Camera->GetPosition().x;
		cam_pos.vector4_f32[1] = m_Camera->GetPosition().y;
		cam_pos.vector4_f32[2] = m_Camera->GetPosition().z;
		cam_pos.vector4_f32[3] = 1.f;
#endif // !_XM_NO_INTRINSICS_


		ModelShader* model_shader = reinterpret_cast<ModelShader*>(m_Shader);
		Material* material = model->GetModelNode()->GetModel()->GetMaterial();
		model_shader->SetShaderResource(eTextureType::ALBEDO, material->GetTexture(eTextureType::ALBEDO));
		model_shader->SetShaderResource(eTextureType::NORMAL, material->GetTexture(eTextureType::NORMAL));
		model_shader->SetShaderResource(eTextureType::AMBIENT_OCCLUSION, material->GetTexture(eTextureType::AMBIENT_OCCLUSION));
		model_shader->SetShaderResource(eTextureType::ROUGHNESS, material->GetTexture(eTextureType::ROUGHNESS));
		model_shader->SetShaderResource(eTextureType::SUBSTANCE, material->GetTexture(eTextureType::SUBSTANCE));
		
		m_Shader->Apply(d3dinterface.GetContext());

		const bool succeded = m_Shader->SetShaderVars(d3dinterface.GetContext(),
														{modelWorldMatrix,
														cameraWorldMatrix,
														projectionMatrix,
														cam_pos, 
														m_Dt});
		ASSERT(succeded, "shader failed to SetShaderVars!");
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
	camera_ori = m_Camera->GetWorldOrientation();
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

	current_transform.Get(modelWorldMatrix);

	if (m_Camera != nullptr)
	{
		DirectX::XMVECTOR cam_pos;
#ifndef _XM_NO_INTRINSICS_
		cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
		cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
		cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
		cam_pos.m128_f32[3] = 1.f;
#else
		cam_pos.vector4_f32[0] = m_Camera->GetPosition().x;
		cam_pos.vector4_f32[1] = m_Camera->GetPosition().y;
		cam_pos.vector4_f32[2] = m_Camera->GetPosition().z;
		cam_pos.vector4_f32[3] = 1.f;
#endif // !_XM_NO_INTRINSICS_

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
	m_Camera->UpdateProjMatrix();
	ZMasher::Matrix44f camera_ori;
	
	ZMasher::Matrix44f world = m_Camera->GetWorldOrientation();
	world.SetTranslation(ZMasher::Vector4f(m_Camera->GetPosition(),1));
	const ZMasher::Matrix44f view_matrix = ~world;

	DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
	cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	m_Terrain->GetModelNode()->GetModel()->SetRenderVars(d3dinterface.GetContext());//TODO: replace this with lazy update

	ZMasher::Matrix44f current_transform;
	const float length = 500.f;
	for (byte i = 0; i < 4; i++)
	{
		current_transform = m_Terrain->GetTransform();
		if (i==0)
		{
			current_transform.m_Vectors[3].x = -length;
			current_transform.m_Vectors[3].z = length;
		}
		else if (i==1)
		{
			current_transform.m_Vectors[3].x = length;
			current_transform.m_Vectors[3].z = length;
		}
		else if (i==2)
		{
			current_transform.m_Vectors[3].x = -length;
			current_transform.m_Vectors[3].z = -length;
		}
		else if (i==3)
		{
			current_transform.m_Vectors[3].x = length;
			current_transform.m_Vectors[3].z = -length;
		}

		current_transform.Get(modelWorldMatrix);

		DirectX::XMVECTOR cam_pos;
#ifndef _XM_NO_INTRINSICS_
		cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
		cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
		cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
		cam_pos.m128_f32[3] = 1.f;
#else
		cam_pos.vector4_f32[0] = m_Camera->GetPosition().x;
		cam_pos.vector4_f32[1] = m_Camera->GetPosition().y;
		cam_pos.vector4_f32[2] = m_Camera->GetPosition().z;
		cam_pos.vector4_f32[3] = 1.f;
#endif // !_XM_NO_INTRINSICS_


		const bool succeded = m_TerrainShader->SetShaderVars(d3dinterface.GetContext(),
															 { modelWorldMatrix,
															 cameraWorldMatrix,
															 projectionMatrix,
															 cam_pos,
															 m_Dt });
		ASSERT(succeded, "shader failed to init!");
		ModelShader* model_shader = reinterpret_cast<ModelShader*>(m_TerrainShader);
		Material* material = m_Terrain->GetModelNode()->GetModel()->GetMaterial();

		model_shader->SetShaderResource(eTextureType::ALBEDO, material->GetTexture(eTextureType::ALBEDO));

		m_TerrainShader->Apply(d3dinterface.GetContext());

		d3dinterface.GetContext()->DrawIndexed(m_Terrain->GetModelNode()->GetModel()->GetIndexCount(), 0, 0);
	}
}

void ZMRenderer::RenderDebugLines(ZMD3DInterface& d3dinterface)
{
	m_Camera->UpdateProjMatrix();
	ZMasher::Matrix44f camera_ori;
	
	ZMasher::Matrix44f world = m_Camera->GetWorldOrientation();
	world.SetTranslation(ZMasher::Vector4f(m_Camera->GetPosition(),1));
	const ZMasher::Matrix44f view_matrix = ~world;

	DirectX::XMMATRIX modelWorldMatrix, cameraWorldMatrix, projectionMatrix;
	cameraWorldMatrix = DirectX::XMMATRIX(&view_matrix.m_Elements[0][0]);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	ZMasher::Matrix44f current_transform;
	current_transform = m_Terrain->GetTransform();
	current_transform.Get(modelWorldMatrix);

	DirectX::XMVECTOR cam_pos;
#ifndef _XM_NO_INTRINSICS_
	cam_pos.m128_f32[0] = m_Camera->GetPosition().x;
	cam_pos.m128_f32[1] = m_Camera->GetPosition().y;
	cam_pos.m128_f32[2] = m_Camera->GetPosition().z;
	cam_pos.m128_f32[3] = 1.f;
#else 
	cam_pos.vector4_f32[0] = m_Camera->GetPosition().x;
	cam_pos.vector4_f32[1] = m_Camera->GetPosition().y;
	cam_pos.vector4_f32[2] = m_Camera->GetPosition().z;
	cam_pos.vector4_f32[3] = 1.f;
#endif // !

	const bool succeded = m_DebugLineShader->SetShaderVars(d3dinterface.GetContext(),
														{modelWorldMatrix,
														cameraWorldMatrix,
														projectionMatrix,
														cam_pos, 
														m_Dt});
	ASSERT(succeded, "shader failed to init!");

	GrowArray<DebugLineInfo, int, 1024*1024>& debugLines = ZMModelFactory::Instance()->m_DebugLines;
	for (int i = 0; i < debugLines.Size(); ++i)
	{
		DWORD colour;
		switch (debugLines[i].m_Colour)
		{
		case eColour::BLUE:
			colour = txtBlue;
			break;
		case eColour::RED:
			colour = txtRed;
			break;
		case eColour::GREEN:
			colour = txtGreen;
			break;
		case eColour::YELLOW:
			colour = txtYellow;
			break;
		case eColour::ORANGE:
			colour = txtOrange;
			break;
		case eColour::WHITE:
			colour = txtWhite;
			break;
		case eColour::BLACK:
			colour = txtBlack;
			break;
		case eColour::GREY:
			colour = txtGrey;
			break;
		case eColour::BLUE2:
			colour = SpecialBlue;
			break;
		case eColour::PINK:
			colour = txtPink;
			break;
		case eColour::TUR:
			colour = txtTur;
			break;
		case eColour::GREEN2:
			colour = EspGreen;
			break;
		case eColour::GREY2:
			colour = EspGrey;
			break;
		default:
			break;
		}

		int a = (colour >> 24) & 0xff;
		int r = (colour >> 16) & 0xff;
		int g = (colour >> 8) & 0xff;
		int b = (colour) & 0xff;
		
		ZMasher::Vector4f colour4f;
		colour4f.x = ( ( FLOAT ) r / 255.0f);
		colour4f.y = ( ( FLOAT ) g / 255.0f);
		colour4f.z = ( ( FLOAT ) b / 255.0f);
		colour4f.w = ( ( FLOAT ) a / 255.0f);

		m_DebugLine->SetPositions(debugLines[i].a-m_Camera->GetPosition(), debugLines[i].b-m_Camera->GetPosition());
		m_DebugLine->SetColour(colour4f);

		m_DebugLine->Apply(d3dinterface.GetContext());

		DebugLineShader* dlShader = reinterpret_cast<DebugLineShader*>( m_DebugLineShader);

		dlShader->Apply(d3dinterface.GetContext());

		d3dinterface.GetContext()->Draw( 2, 0 );
	}
	debugLines.RemoveAll();
}