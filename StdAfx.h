// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define	MOUSE_BUFFER_SIZE 16

#define _USE_MATH_DEFINES
#include <math.h>

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
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

// Global defines
const int RES_X = 1024;
const int RES_Y = 768;

const float BOARD_L = 0.025f;				// left
const float BOARD_R = 0.975f;				// right
const float BOARD_T = 0.00f;				// top
const float BOARD_B = 0.75f;				// bottom

const float BOARD_W	= (BOARD_R-BOARD_L);	//width
const float BOARD_H	= (BOARD_B-BOARD_T);	//height

const float BONUS_PROB = 0.3f;

const float MIN_BALL_SIZE = 1.0f / 128;
const float MAX_BALL_SIZE = 1.0f / 48;
const float AVG_BALL_SIZE = (MIN_BALL_SIZE + MAX_BALL_SIZE) / 2;
//TODO: œrednia geometryczna

const float MIN_BALL_SPEED = 0.25f;
const float MAX_BALL_SPEED = 0.75f;
const float AVG_BALL_SPEED = (MIN_BALL_SPEED + MAX_BALL_SPEED) / 2;

const float MIN_PADDLE_WIDTH = BOARD_W / 16;
const float MAX_PADDLE_WIDTH = BOARD_W / 2;
const float AVG_PADDLE_WIDTH = (MIN_PADDLE_WIDTH + MAX_PADDLE_WIDTH) / 2;

inline float frand( float i, float j ) 
	{ return ((float)rand()/RAND_MAX)*(j-i)+i; };
