#pragma once
#include "d3dappstage.h"
#include "MovingSprite.h"

class CGameMenu : public CD3DAppStage
{
public:
	CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CGameMenu();

	HRESULT RestoreDeviceObjects();
	HRESULT FrameMove( FLOAT fElapsedTime );
	HRESULT Render();
	HRESULT InvalidateDeviceObjects();

	LPDIRECT3DTEXTURE8		pTex[256];
	LPD3DXSPRITE			pSprite;

	list<CSprite*>			listRender;
	list<CMovingSprite*>	listFrameMove;
};
