#pragma once
#include <dinput.h>
#include <list>
#include "movingsprite.h"
#include "menuitem.h"

using namespace std;

class CCursor :
	public CMovingSprite
{
public:
	CCursor( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice, list<CMenuItem*>* pListMenuItem );
	virtual ~CCursor();

	void FrameMove( FLOAT fElapsedTime );
	void Render( LPD3DXSPRITE pSprite ) const;

private:
	LPDIRECTINPUTDEVICE8	pDIDevice;
	list<CMenuItem*>*		pListMenuItem;
};
