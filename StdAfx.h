// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Global defines
#define RES_X		1024
#define RES_Y		768

// TODO: reference additional headers your program requires here

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
//#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define DIRECTINPUT_VERSION  0x0800
#include <dxerr8.h>

#include <list>
using namespace std;

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

inline float frand( float i, float j ) {
	return ((float)rand()/RAND_MAX)*(j-i)+i;
};
