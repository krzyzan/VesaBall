#pragma once
#include "D3DScene.h"
#include "BrickArray.h"


class CGameBoard :
	public CD3DScene
{
public:
	CGameBoard( LPDIRECT3DDEVICE8 d3dDevice );
	virtual ~CGameBoard(void);

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );

	HRESULT FrameRender();

	CD3DScene* GetNextScene();
	
protected:
	LPD3DXSPRITE		pSprite;
	
	list<CSprite*>		listSprite;
	CBrickArray*		pBrickArray;
	DWORD				dwLevelNum;

private:
	bool		bQuit;
};
