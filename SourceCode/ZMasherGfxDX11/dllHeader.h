#pragma once

//#define ZMASHER_DLL_EXPORT
#ifdef ZMASHER_GFX_DX11_DLL_EXPORT
#define ZMASHER_GFX_DX11_DLL __declspec(dllexport)
#else 
#define ZMASHER_GFX_DX11_DLL __declspec(dllimport)
#endif