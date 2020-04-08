#include "ZMModelFactory.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <ZMUtils\Math\ZMVector4.h>
#include <ZMUtils\DataStructures\GrowArray.h>
#include <ZMasherGfxDX11\ZMModelInstanceNode.h>
#include <ZMasherGfxDX11\ZMModelNode.h>
#include <tinyxml2.h>
#include <Debugging\ZMDebugger.h>
#include <MemoryManager.h>
#include <GlobalIncludes\project_defines.h>
#include <ZMUtils\File\PathManager.h>
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
#ifdef NEW_OP_OVERRIDE
	FbxSetMallocHandler(ZMAlloc);
	FbxSetFreeHandler(ZMFree);
	FbxSetReallocHandler(ZMRealloc);
	FbxSetCallocHandler(ZMCalloc);  
#endif // NEW_OP_OVERRIDE


	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	if (lImporter == nullptr ||
		pManager->GetIOSettings() == nullptr)
	{
		//ZMDebugger::Instance()->OutputDebugMsg("lImporter IS NULLPTR!!!");
	}
	printf_s("\nlImporter ptr: %i", lImporter);
	printf_s("\npManager ptr: %i", pManager);
	printf_s("\npManager->GetIOSettings() ptr: %i", pManager->GetIOSettings());
	// Initialize the importer by providing a filename.

	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		//note to self: "Unexpected file type" means "general error", it could be exactly anything
		//even that it is the incorrect path (why they simply don't say that is a mystery to me)
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}
	FbxGeometryConverter lGeomConverter(pManager);
	lGeomConverter.Triangulate(reinterpret_cast<fbxsdk::FbxScene*>(pScene), /*replace*/true);
	lGeomConverter.SplitMeshesPerMaterial(reinterpret_cast<fbxsdk::FbxScene*>(pScene), /*replace*/true);

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

ZMModelFactory::ZMModelFactory()
{
	m_ModelInstances.Resize(1024);
	m_Materials.Resize(1024);
	InitializeSdkObjects(m_FbxManager, m_Scene);
}

ZMModelInstanceNode* ZMModelFactory::InitModelInstanceNode(ZMModelNode* model_node, ZMModelInstanceNode* instance_node)
{
	void* model_mem = _mm_malloc(sizeof(ZMModelInstanceNode), 16);
	ZMModelInstanceNode* new_node = new ZMModelInstanceNode();
	if (instance_node != nullptr)
	{
		instance_node->AddModelInstanceNode(new_node);
	}
	new_node->SetModelNode(model_node);

	for (short i = 0; i < model_node->ChildCount(); ++i)
	{
		InitModelInstanceNode(model_node->GetChild(i), new_node);
	}
	return new_node;
}

const char* MODELFACTORY_ALBEDO_TEXTURE_KEY = "albedoTexture";
const char* MODELFACTORY_MODEL_KEY = "model";
const char* MODELFACTORY_FILEPATH_KEY = "filepath";

const char* MODEL_SUFF = "fbx";

const char* TEXTURE_FILETYPE = "dds";

const char* ALBEDO_SUFF = "al";
const char* NORMAL_SUFF = "no";
const char* AO_SUFF = "ao";
const char* ROUGHNESS_SUFF = "ro";
const char* SUBSTANCE_SUFF = "su";


ZMModelInstanceNode* ZMModelFactory::LoadModelInstance(const char * model_path)
{
	// TODO: 
	// Don't read an xml file every single time we're loading a model -.-
	// it might already be loaded
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error_code = doc.LoadFile(model_path);
	if (error_code != tinyxml2::XMLError::XML_SUCCESS)
	{
		const std::string error_msg = tinyxml2::XMLDocument::ErrorIDToName(error_code);
		ASSERT(false, std::string("Failed to load: " + std::string(model_path) + ". Error message: " + error_msg).c_str());
		return nullptr;
	}
	tinyxml2::XMLElement* root = doc.RootElement();
	std::string model_xml_path;
	for (tinyxml2::XMLElement* it = root->FirstChildElement(); 
			it != nullptr; 
			it = it->NextSiblingElement())
	{
		if (std::strcmp(it->Name(), MODELFACTORY_MODEL_KEY) == 0)
		{
			model_xml_path = PathManager::Instance()->GetDataPath() + it->Attribute(MODELFACTORY_FILEPATH_KEY);
		}
	}
	
	ZMModelNode* model_node = nullptr;

	for (short i = 0; i < m_ModelNodes.Size(); i++)
	{
		if (std::strcmp(m_ModelNodes[i]->m_UglyName.c_str(), model_path) == 0)
		{
			model_node = m_ModelNodes[i];
			break;
		}
	}

	if (model_node == nullptr)
	{
		const bool result = LoadScene(m_FbxManager, m_Scene, (model_xml_path + "." + MODEL_SUFF).c_str());
		ASSERT(result, "ZMModelFactory::LoadModelInstance Failed to init");

		model_node = new ZMModelNode(nullptr);
		m_ModelNodes.Add(model_node);
		model_node->m_UglyName = model_path;
		model_node = ProcessMeshHierarchy(m_Scene->GetRootNode(), model_xml_path, model_node);
		
		if (model_node == nullptr)
		{
			ASSERT(false, "Model failed to load!");
			return nullptr;
		}
	}
	ZMModelInstanceNode* node = InitModelInstanceNode(model_node);
	m_ModelInstances.Add(node);

	return node;
}

//TODO: refactor this function, very hard to navigate in atm
ZMModelNode* ZMModelFactory::ProcessMeshHierarchy(FbxNode* inNode, const std::string& model_xml_path, ZMModelNode* parent)
{
	FbxMesh* mesh = inNode->GetMesh();
	//no mesh to be found in this node, keep going with children instead
	if (mesh == nullptr)
	{
		for (short i = 0; i < inNode->GetChildCount(); ++i)
		{
			ProcessMeshHierarchy(inNode->GetChild(i), model_xml_path, parent);
		}
		return parent;
	}
	
	std::vector<ZMasher::Vector4f> control_points;
	control_points.reserve(1024);
	for (short i = 0; i < mesh->GetControlPointsCount(); i++)
	{
		ZMasher::Vector4f cp;
		cp.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		cp.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		cp.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
		cp.w = 0.f;
		control_points.push_back(ZMasher::Vector4f(cp));
	}

	const int tri_count = mesh->GetPolygonCount();
	const int index_count = tri_count * 3;
	const int vertex_count = tri_count * 3;

	unsigned long* indexes = new unsigned long[index_count]; //TODO: Allocate in buffers, note: not so important since these will be freed after loading
	CurrentVertexType* vertexes = new CurrentVertexType[vertex_count];//TODO: same as above
	const FbxGeometryElementNormal * lNormalElement = mesh->GetElementNormal(0);
	ASSERT(lNormalElement, "Normalelement nullptr!");
	int vertex_counter = 0;
	FbxStringList list;
	mesh->GetUVSetNames(list);
	FbxArray<FbxVector2> uv_array;
	mesh->GetPolygonVertexUVs(list[0], uv_array);
	for (short i = 0; i < tri_count; ++i)
	{
		for (short j = 0; j < 3; ++j)
		{
			int cp_index = 0;
			cp_index = mesh->GetPolygonVertex(i, j);

			ZMasher::Vector4f cp = control_points[cp_index];

			CurrentVertexType temp;
			temp.m_Position.x = cp.x;
			temp.m_Position.y = cp.y;
			temp.m_Position.z = cp.z;
			temp.m_Position.w = 0;

			FbxVector4 normal;
			const bool normal_result = mesh->GetPolygonVertexNormal(i, j, normal);
			ASSERT(normal_result, "Failed to get normal!");
			temp.m_Normal.x = normal[0];
			temp.m_Normal.y = normal[1];
			temp.m_Normal.z = normal[2];
			temp.m_Normal.w = 0;
			FbxVector2 uv;

			bool unmapped = true;

			const bool uv_result = mesh->GetPolygonVertexUV(i, j, list[0], uv, unmapped);
			ASSERT(uv_result, "Failed to get UV!");
			temp.m_Tex.x = uv[0];
			temp.m_Tex.y = uv[1]*-1.f;
			
			vertexes[vertex_counter] = temp;
			indexes[vertex_counter] = vertex_counter;
			++vertex_counter;
		}
	}

	ZMModel* model = AddModel();

	if (model->CreateModel(m_Device, vertexes, indexes, vertex_count, index_count, &m_Materials.GetLast()) == false)
	{
		//something's fucky -.-
		ASSERT(false, "CreateModel failed!");
		return nullptr;
	}
	{
		std::string model_path_str = model_xml_path + std::string("_");
		Texture* temp_texture = m_TextureContainer.GetTexture((model_path_str + ALBEDO_SUFF + "." + TEXTURE_FILETYPE).c_str());
		if (temp_texture)
		{
			model->GetMaterial()->AddTexture(eTextureType::ALBEDO, temp_texture->GetResourceView());
		}

		temp_texture = m_TextureContainer.GetTexture((model_path_str + NORMAL_SUFF + "." + TEXTURE_FILETYPE).c_str());
		if (temp_texture)
		{
			model->GetMaterial()->AddTexture(eTextureType::NORMAL, temp_texture->GetResourceView());
		}

		temp_texture = m_TextureContainer.GetTexture((model_path_str + ROUGHNESS_SUFF + "." + TEXTURE_FILETYPE).c_str());
		if (temp_texture)
		{
			model->GetMaterial()->AddTexture(eTextureType::ROUGHNESS, temp_texture->GetResourceView());
		}

		temp_texture = m_TextureContainer.GetTexture((model_path_str + AO_SUFF + "." + TEXTURE_FILETYPE).c_str());
		if (temp_texture)
		{
			model->GetMaterial()->AddTexture(eTextureType::AMBIENT_OCCLUSION, temp_texture->GetResourceView());
		}

		temp_texture = m_TextureContainer.GetTexture((model_path_str + SUBSTANCE_SUFF + "." + TEXTURE_FILETYPE).c_str());
		if (temp_texture)
		{
			model->GetMaterial()->AddTexture(eTextureType::SUBSTANCE, temp_texture->GetResourceView());
		}
	}

	ZMModelNode* model_node = new ZMModelNode(model);//TODO: don't do new here
	if (parent != nullptr)
	{
		parent->AddChild(model_node);
	}

	for (short i = 0; i < inNode->GetChildCount(); ++i)
	{
		ProcessMeshHierarchy(inNode->GetChild(i), model_xml_path, model_node);
	}

	//It's seems like the relative position of the models are saved in nodes called "pose". 
	//Task is, get the poses and apply them to children accordingly

	fbxsdk::FbxMatrix node_transform = inNode->EvaluateGlobalTransform(0);
#define TO_FLOAT(d) static_cast<float>(d)
	ZMasher::Matrix44f model_transform(TO_FLOAT(node_transform.Get(0, 0)), TO_FLOAT(node_transform.Get(0, 1)), TO_FLOAT(node_transform.Get(0, 2)), TO_FLOAT(node_transform.Get(0, 3)),
									   TO_FLOAT(node_transform.Get(1, 0)), TO_FLOAT(node_transform.Get(1, 1)), TO_FLOAT(node_transform.Get(1, 2)), TO_FLOAT(node_transform.Get(1, 3)),
									   TO_FLOAT(node_transform.Get(2, 0)), TO_FLOAT(node_transform.Get(2, 1)), TO_FLOAT(node_transform.Get(2, 2)), TO_FLOAT(node_transform.Get(2, 3)),
									   TO_FLOAT(node_transform.Get(3, 0)), TO_FLOAT(node_transform.Get(3, 1)), TO_FLOAT(node_transform.Get(3, 2)), TO_FLOAT(node_transform.Get(3, 3)));
	model_node->SetTransform(model_transform);
	return model_node;
}

ZMModel* ZMModelFactory::AddModel()
{
	ZMModel* model = new ZMModel();//TODO: Oh jesus fuck do something about this ASAP
	m_Materials.Add(Material());
	return model;
}

ZMModelInstanceNode* ZMModelFactory::LoadSkyBox(const char* skybox_path)
{
	ZMModel* model = AddModel();

	unsigned long* indexes= nullptr;
	CurrentVertexType* vertexes = nullptr;

	model->CreateCube(m_Device, vertexes, indexes, &m_Materials.GetLast());
	Texture* temp_texture = m_TextureContainer.GetTexture(skybox_path);
	model->GetMaterial()->AddTexture(eTextureType::ALBEDO, temp_texture->GetResourceView());
	return InitModelInstanceNode(new ZMModelNode(model));
}

ZMModelInstanceNode* ZMModelFactory::Load2DTerrain(	const char* texture_path,
													const float height,
													const float width,
													const float length)
{
	ZMModel* model = AddModel();

	CurrentVertexType* vertices = nullptr;
	unsigned long* indices = nullptr;

	model->CreatePlaneHorizontal(m_Device, vertices, indices, &m_Materials.GetLast());
	Texture* temp_texture = m_TextureContainer.GetTexture(texture_path);
	model->GetMaterial()->AddTexture(eTextureType::ALBEDO, temp_texture->GetResourceView());
	return InitModelInstanceNode(new ZMModelNode(model));
}

DebugLineInfo* ZMModelFactory::CreateDebugLine(const ZMasher::Vector3f & point_a, const ZMasher::Vector3f & point_b, eColour colour)
{
	DebugLineInfo* debugLine = new DebugLineInfo{point_a, point_b, colour};
	m_DebugLines.Add(debugLine);
	return debugLine;
}

void ZMModelFactory::RemoveDebugLine(DebugLineInfo* debugLine)
{
	auto index = m_DebugLines.Find(debugLine);
	if(index != m_DebugLines.found_none)
	{
		m_DebugLines.DeleteCyclic(index);
	}
}