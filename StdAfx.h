// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <list>
#include <vector>
using namespace std;

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>

#define DIRECTINPUT_VERSION  0x0800
#include <dxerr8.h>
#include <d3dx8.h>

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// Global defines
#define RES_X		1024
#define RES_Y		768

#define BOARD_L	0.05f				// left
#define BOARD_R	0.95f				// right
#define BOARD_T	0.05f				// top
#define BOARD_B	0.75f				// bottom

#define BOARD_W	(BOARD_R-BOARD_L)	//width
#define BOARD_H	(BOARD_B-BOARD_T)	//height

inline float frand( float i, float j ) 
	{ return ((float)rand()/RAND_MAX)*(j-i)+i; };
