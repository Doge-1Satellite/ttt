﻿
#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/RELEASE")
#ifdef _MERGE_RDATA_
#pragma comment(linker,"/merge:.rdata=.data")
#endif
#pragma comment(linker,"/merge:.text=.data")
#pragma comment(linker,"/merge:.reloc=.data")
#if _MSC_VER >= 1000
#endif
#endif
#pragma comment(linker,"/MERGE:.rdata=.data")
#pragma comment(linker,"/MERGE:.text=.data")
//#pragma comment(lib,"msvcrt.lib")
#if (_MSC_VER < 1300) 
#pragma comment(linker,"/IGNORE:4078") 
#pragma comment(linker,"/OPT:NOWIN98")
 
#endif
#define WIN32_LEAN_AND_MEAN
