#pragma once

#include "d3dappscene.h"

class CMovingSprite;
class CMenuItem;


class CGameMenu :
	public CD3DAppScene
{
public:
	CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CGameMenu();

	HRESULT InitDeviceObjects();
	HRESULT RenderLoop();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT AddMenuTitle( LPDIRECT3DTEXTURE8 pTex, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position, D3DCOLOR Blending );
	HRESULT AddMenuItem( LPDIRECT3DTEXTURE8 pTex, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID );
	HRESULT AddCursor( LPDIRECT3DTEXTURE8 pTex );

	LPD3DXSPRITE				pSprite;

	list<CMovingSprite*>		listFrameMove;
	list<CMenuItem*>			listMenuItem;
};
