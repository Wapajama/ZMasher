#pragma once

//#define BENCHMARK

//#define ZMASHER_DX12
//#ifdef WIN64
//typedef unsigned __int64 ZM_Mem_Size;
//#elif
//typedef size_t ZM_Mem_Size;
//#endif // WIN64

#define BENCHMARK

#ifdef BENCHMARK
#define START_TIME_STAMP(ts) ts.StartTimeStamp() 
#define END_TIME_STAMP(ts) ts.EndTimeStamp() 
#else
#define START_TIME_STAMP(ts)
#define END_TIME_STAMP(ts)
#endif // BENCHMARK


#ifndef ZMASHER_DX12
#define ZMASHER_DX11  
#endif // !ZMASHER_DX12

#ifdef ZMASHER_DX11

#endif

#define NEW_OP_OVERRIDE
