#pragma once
#include "GameBoard.h"

class CCursor;

class CGameEditor :
	public CGameBoard
{
public:
	CGameEditor();
	virtual ~CGameEditor();

	HRESULT InitDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod );
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();

private:
	CCursor*	pCursor;
	DWORD		curType;
};
