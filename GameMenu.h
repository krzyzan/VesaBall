#pragma once

#include <list>
using namespace std;

#include "d3dappscene.h"

class CMenuItem;
class CCursor;


class CGameMenu :
	public CD3DAppScene
{
public:
	enum UID_MSG {
		UID_START	= 100,
		UID_EDITOR,
		UID_QUIT,
		UID_GAME_OVER		//TODO: TMP
	};

	CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CGameMenu();

	HRESULT InitDeviceObjects();
	HRESULT RenderLoop();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	CMenuItem*			GetPointedMenuItem();

	LPD3DXSPRITE		pSprite;

	list<CSprite*>		listRender;
	list<CMenuItem*>	listMenuItem;
	CCursor*			pCursor;

	CMenuItem*			pOldMenuItem;
	CMenuItem*			pPressedMenuItem;
};
