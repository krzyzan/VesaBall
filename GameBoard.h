#pragma once
#include "D3DAppScene.h"
#include "BrickArray.h"


class CGameBoard :
	public CD3DAppScene
{
public:
	CGameBoard( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CGameBoard(void);

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT RenderLoop();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

protected:
	LPD3DXSPRITE		pSprite;
	
	list<CSprite*>		listSprite;
	CBrickArray*		pBrickArray;
};
