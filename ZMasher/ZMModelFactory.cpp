#include "ZMModelFactory.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <ZMUtils\Math\ZMVector4.h>
#include <ZMUtils\DataStructures\GrowArray.h>
#include <ZMasher\ZMModelInstanceNode.h>
#include <ZMasher\ZMModelNode.h>

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
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

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		//note to self: "Unexpected file type" means "general error", it could be exactely anything
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

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

ZMModelFactory::ZMModelFactory() : m_Models(0)
{
	m_ModelInstances.Resize(1024);

	InitializeSdkObjects(m_FbxManager, m_Scene);
}

ZMModelInstanceNode* ZMModelFactory::InitModelInstanceNode(ZMModelNode* model_node, ZMModelInstanceNode* instance_node)
{
	ZMModelInstanceNode* new_node = new ZMModelInstanceNode();
	if (instance_node != nullptr)
	{
		instance_node->AddModelInstanceNode(new_node);
	}
	new_node->SetModelNode(model_node);
	//new_node->SetModel(model_node->GetModel());
	ZMasher::Vector3f position;

	new_node->SetPosition(position);
	for (short i = 0; i < model_node->ChildCount(); ++i)
	{
		InitModelInstanceNode(model_node->GetChild(i), new_node);
	}
	return new_node;
}

ZMModelInstanceNode* ZMModelFactory::LoadModelInstance(ID3D11Device* device, const char * model_path)
{
	const bool result = LoadScene(m_FbxManager, m_Scene, model_path);
	ASSERT(result, "ZMModelFactory::LoadModelInstance Failed to init");

	ZMModelNode* model_node = new ZMModelNode(nullptr);
	FbxArray<FbxPose*> pose_array;
	m_Scene->FillPoseArray(pose_array);

	model_node = ProcessMeshHierarchy(device, m_Scene->GetRootNode(), model_node);
	if (model_node == nullptr)
	{
		ASSERT(false, "DERP");
	}
	return InitModelInstanceNode(model_node);
}

bool findTexture(FbxNode* inNode)
{
	for (short i = 0; i < inNode->GetChildCount(); i++)
	{
		//if (inNode->GetMater)
		{

		}
	}

	return false;
}

ZMModelNode* ZMModelFactory::ProcessMeshHierarchy(ID3D11Device* device, FbxNode* inNode, ZMModelNode* parent)
{
	FbxMesh* mesh = inNode->GetMesh();
	//no mesh to be found in this node, keep going with children instead
	if (mesh == nullptr)
	{
		for (short i = 0; i < inNode->GetChildCount(); ++i)
		{
			ProcessMeshHierarchy(device, inNode->GetChild(i), parent);
		}
		return parent;
	}
	
	

	std::vector<ZMasher::Vector4f> control_points;

	for (short i = 0; i < mesh->GetControlPointsCount(); i++)
	{
		ZMasher::Vector4f cp;
		cp.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		cp.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		cp.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
		cp.w = 0.f;
		control_points.push_back(cp);
	}

	int tri_count = mesh->GetPolygonCount();
	int index_count = mesh->GetPolygonCount() * 3;
	int vertex_count = mesh->GetPolygonCount() * 3;

	unsigned long* indexes = new unsigned long[index_count];
	CurrentVertexType* vertexes = new CurrentVertexType[vertex_count];
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

			FbxVector4 normal;
			const bool normal_result = mesh->GetPolygonVertexNormal(i, j, normal);
			ASSERT(normal_result, "Failed to get normal!");
			temp.m_Normal.x = normal[0];
			temp.m_Normal.y = normal[1];
			temp.m_Normal.z = normal[2];
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

	ZMModel* model = new ZMModel();

	if (model->CreateModel(device, vertexes, indexes, vertex_count, index_count) == false)
	{
		//something's fucky -.-
		ASSERT(false, "CreateModel failed!");
		return nullptr;
	}

	ZMModelNode* model_node = new ZMModelNode(model);
	if (parent != nullptr)
	{
		parent->AddChild(model_node);
	}

	for (short i = 0; i < inNode->GetChildCount(); ++i)
	{
		ProcessMeshHierarchy(device, inNode->GetChild(i), model_node);
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
