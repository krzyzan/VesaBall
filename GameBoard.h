#pragma once
#include "D3DScene.h"
#include "BrickArray.h"


class CGameBoard :
	public CD3DScene
{
public:
	CGameBoard();
	virtual ~CGameBoard();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );

	HRESULT FrameRender();

protected:
	LPD3DXSPRITE		pSprite;
	
	list<CSprite*>		listSprite;
	CBrickArray*		pBrickArray;
	DWORD				dwLevelNum;
};
