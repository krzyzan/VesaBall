#pragma once
#include "GameBoard.h"

class CCursor;

class CGameEditor :
	public CGameBoard
{
public:
	CGameEditor( LPDIRECT3DDEVICE8 d3dDevice );
	virtual ~CGameEditor();

	HRESULT InitDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod );
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();

	CD3DAppScene* GetNextScene();

protected:
	CCursor*	pCursor;
	DWORD		curType;
};
