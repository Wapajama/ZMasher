#pragma once

//#define ZMASHER_DLL_EXPORT
#ifdef ZMASHER_DLL_EXPORT
#define ZMASHER_DLL __declspec(dllexport)
#else 
#define ZMASHER_DLL __declspec(dllimport)
#endif