// D3DBallApp.h: interface for the CD3DBallApp class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx8tex.h>
#include "D3DApp.h"

class CD3DBallApp : public CD3DApp
{
public:
	CD3DBallApp( HINSTANCE hInstance );
	virtual ~CD3DBallApp();

protected:
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT FrameMove();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT Render();

	LPDIRECT3DTEXTURE8		pTex;
	LPD3DXSPRITE			pSprite;
};

