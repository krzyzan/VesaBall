#pragma once
#include "D3DAppScene.h"
#include "BrickArray.h"


class CGameBoard :
	public CD3DAppScene
{
public:
	CGameBoard( LPDIRECT3DDEVICE8 d3dDevice );
	virtual ~CGameBoard(void);

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT FrameRender();

protected:
	LPD3DXSPRITE		pSprite;
	
	list<CSprite*>		listSprite;
	CBrickArray*		pBrickArray;
};
