#pragma once

#include <list>
using namespace std;

#include <dinput.h>

#include "movingsprite.h"

class CMenuItem;

class CCursor :
	public CMovingSprite
{
public:
	CCursor( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice, list<CMenuItem*>* pListMenuItem );
	virtual ~CCursor();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void Render( LPD3DXSPRITE pSprite ) const;

private:
	LPDIRECTINPUTDEVICE8	pDIDevice;
	list<CMenuItem*>*		pListMenuItem;
	CMenuItem*				pOldMenuItem;
	CMenuItem*				pPressedMenuItem;
	BOOL					bOldButtonState;
};
