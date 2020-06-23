#pragma once
#include <Utility/ZMSingleton.h>
#include "ZMModel.h"
#include <string>
#include <DataStructures\GrowArray.h>
#include <fbxsdk.h>
#include <DataStructures\MemoryContainer.h>
#include <ZMasherGfxDX11\TextureContainer.h>
#include <ZMasherGfxDX11\Material.h>
#include <ZMasherGfxDX11\dllHeader.h>
#include <GameObject.h>

class ZMModelInstance;
class ZMModel;
class ID3D11Device;
class ZMModelInstanceNode;
class ZMModelNode;

namespace std
{
	typedef ifstream;
}

enum eColour
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	WHITE,
	BLACK,
	GREY,
	BLUE2,
	PINK,
	TUR,
	GREEN2,
	GREY2,
	N_COLOURS
};

struct DebugLineInfo
{
	ZMasher::Vector3f a;
	ZMasher::Vector3f b;
	eColour m_Colour;
	int id;
};

#ifndef D3DCOLOR_ARGB
#define D3DCOLOR_ARGB(a,r,g,b) \
	((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#endif

const DWORD txtRed			= D3DCOLOR_ARGB( 255,	255,	0,		0	);
const DWORD txtGreen		= D3DCOLOR_ARGB( 255,	0,		255,	0	);
const DWORD txtBlue			= D3DCOLOR_ARGB( 255,	0,		0,		255	);
const DWORD txtYellow		= D3DCOLOR_ARGB( 255,	255,	255,	0	);
const DWORD txtOrange		= D3DCOLOR_ARGB( 255,	255,	100,	0	);
const DWORD txtWhite		= D3DCOLOR_ARGB( 255,	255,	255,	255 );
const DWORD txtBlack		= D3DCOLOR_ARGB( 255,	0,		0,		0	);
const DWORD txtGrey			= D3DCOLOR_ARGB( 255,	86,		86,		86	);
const DWORD SpecialBlue		= D3DCOLOR_ARGB( 255,	51,		102,	255 );
const DWORD txtPink			= D3DCOLOR_ARGB( 255,	255,	0,		255	);
const DWORD txtTur			= D3DCOLOR_ARGB( 255,	150,	0,		255	);
 
const DWORD EspGreen		= D3DCOLOR_ARGB( 255,	15,		200,	15	);
const DWORD EspGrey			= D3DCOLOR_ARGB( 255,	80,		80,		80	);

class ZMASHER_GFX_DX11_DLL ZMModelFactory: public ZMSingleton<ZMModelFactory>
{
	friend class ZMRenderer;//TODO: REMOVE
public:
	ZMModelFactory();
	ZMModelInstanceNode* LoadModelInstance(const char * model_path);
	ZMModelInstanceNode* LoadSkyBox(const char* skybox_path);//TODO: Create a neater implementation of this
	ZMModelInstanceNode* Load2DTerrain(const char* texture_path,
									   const float height = -5.f, 
									   const float width = 1000.f,
									   const float length = 1000.f);//TODO: same as above
	inline void SetDevice(ID3D11Device* device) { m_Device = device; m_TextureContainer.SetDevice(device); }

	int CreateDebugLine(const ZMasher::Vector3f& point_a, const ZMasher::Vector3f& point_b, eColour colour = eColour::RED);

private:

	ZMModelInstanceNode* InitModelInstanceNode(ZMModelNode* root_node, ZMModelInstanceNode* instance_node = nullptr);

	FbxManager* m_FbxManager;
	FbxScene* m_Scene;
	ZMModelNode* ProcessMeshHierarchy(FbxNode* inNode, const std::string& model_path_str, ZMModelNode* parent = nullptr);

	ZMModel* AddModel();

	//data orient this?
	GrowArray<ZMModelInstanceNode*, int, MAX_GAME_OBJS> m_ModelInstances;
	GrowArray<ZMModelNode*> m_ModelNodes;
	GrowArray<DebugLineInfo, int, 1024*1024> m_DebugLines;
	TextureContainer m_TextureContainer;
	GrowArray<Material> m_Materials;
	ID3D11Device* m_Device;

};