#pragma once

#include <d3dx8.h>
#include <dinput.h>

#include <list>
using namespace std;

#include "sprite.h"

class CD3DAppStage
{
public:
	CD3DAppStage( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CD3DAppStage();

	virtual HRESULT InitDeviceObjects()				= 0;
	virtual HRESULT RestoreDeviceObjects()			= 0;
	virtual HRESULT FrameMove( FLOAT fElapsedTime )	= 0;
	virtual HRESULT Render()						= 0;
	virtual HRESULT InvalidateDeviceObjects()		= 0;
	virtual HRESULT DeleteDeviceObjects()			= 0;

	LPDIRECT3DDEVICE8		pd3dDevice;
	LPDIRECTINPUTDEVICE8	pDIDevice;

	list<CSprite*>			listRender;
};
