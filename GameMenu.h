#pragma once
#include "d3dappscene.h"
#include "menuitem.h"
#include "cursor.h"

class CGameMenu :
	public CD3DAppScene
{
public:
	CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CGameMenu();

	HRESULT InitDeviceObjects();
	HRESULT FrameMove( FLOAT fElapsedTime );
	HRESULT Render();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	LPDIRECT3DTEXTURE8		pTex[256];
	LPD3DXSPRITE			pSprite;

	list<CMovingSprite*>	listFrameMove;
	list<CMenuItem*>		listMenuItem;
};
