// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define RES_X		800
#define RES_Y		600
#define	DECK_Y		0.75f - 0.75f * 8 / 768

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define DIRECTINPUT_VERSION  0x0800
#include <dxerr8.h>

#include <list>
using namespace std;

// TODO: reference additional headers your program requires here
