#pragma once
#include "GameBoard.h"

class CCursor;

class CGameEditor :
	public CGameBoard
{
public:
	CGameEditor( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CGameEditor();

	HRESULT InitDeviceObjects();
	HRESULT RenderLoop();
	HRESULT DeleteDeviceObjects();

protected:
	CCursor*	pCursor;
	LONG		curType;
};
