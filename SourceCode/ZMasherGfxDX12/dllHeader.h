#pragma once
#include <project_defines.h>
#ifdef ZMASHER_DX12
	#ifdef ZMASHER_GFX_DX12_DLL_EXPORT
	#define ZMASHER_GFX_DX12_DLL __declspec(dllexport)
	#else 
	#define ZMASHER_GFX_DX12_DLL __declspec(dllimport)
	#endif  
#endif // ZMASHER_DX12
