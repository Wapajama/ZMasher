#pragma once

//#define ZMASHER_DLL_EXPORT
#ifdef ZMASHER_GFX_DX12_DLL_EXPORT
#define ZMASHER_GFX_DX12_DLL __declspec(dllexport)
#else 
#define ZMASHER_GFX_DX12_DLL __declspec(dllimport)
#endif