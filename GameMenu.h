#pragma once

#include <list>
using namespace std;

#include "d3dscene.h"

class CMenuItem;
class CCursor;
class CSprite;


class CGameMenu :
	public CD3DScene
{
public:
	enum UID_MSG {
		UID_START	= 100,
		UID_EDITOR,
		UID_QUIT,
	};

	CGameMenu( LPDIRECT3DDEVICE8 d3dDevice );
	~CGameMenu();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod );
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );

	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();
	
	CD3DScene* GetNextScene();

private:
	CMenuItem*			GetPointedMenuItem() const;

	LPD3DXSPRITE		pSprite;

	list<CSprite*>		listRender;
	list<CMenuItem*>	listMenuItem;
	CCursor*			pCursor;

	CMenuItem*			pCurMenuItem;
	CMenuItem*			pPressedMenuItem;
	CMenuItem*			pClickedMenuItem;
};
