// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STDAFX_H
#define STDAFX_H

#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include <list>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;

#include <SDL.h>

#include "WinTypes.h"

#define SAFE_DELETE(p)  \
	{                   \
		if (p)          \
		{               \
			delete (p); \
			(p) = NULL; \
		}               \
	}
#define SAFE_DELETE_ARRAY(p) \
	{                        \
		if (p)               \
		{                    \
			delete[] (p);    \
			(p) = NULL;      \
		}                    \
	}

// The game's fixed logical render resolution (its sprites are 640x480-era
// pixel art, and the normalized coordinate space below already bakes in a
// 4:3 aspect ratio). The window itself runs fullscreen at the desktop's
// actual resolution; SDL_RenderSetLogicalSize() (see App.cpp) scales this
// fixed 640x480 canvas up to fill it, letterboxing as needed so sprites
// stay crisp and correctly proportioned on any real display.
const DWORD RES_X = 640;
const DWORD RES_Y = 480;

const float BOARD_L = 0.025f;			   // left
const float BOARD_R = 0.975f;			   // right
const float BOARD_T = 0.00f;			   // top
const float BOARD_B = 0.75f;			   // bottom
const float BOARD_W = (BOARD_R - BOARD_L); // width
const float BOARD_H = (BOARD_B - BOARD_T); // height

const float PADDLE_WIDTH_MIN = BOARD_W / 16;
const float PADDLE_WIDTH_MAX = BOARD_W / 2;
const float PADDLE_WIDTH_AVG = (PADDLE_WIDTH_MIN + PADDLE_WIDTH_MAX) / 2;

const float GRAV_ACCEL = 0.25f;

inline float frand(float i, float j)
{
	return ((float)rand() / RAND_MAX) * (j - i) + i;
};

#endif
