// Deck.h: interface for the CDeck class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <dinput.h>
#include "Sprite.h"

class CDeck : public CSprite  
{
public:
	CDeck::CDeck( LPDIRECT3DTEXTURE8 iTexture, LPDIRECTINPUTDEVICE8 ipDIDevice );
	virtual ~CDeck();

	void FrameMove( FLOAT fElapsedTime );
	void Collide( list<CBall*>* pListBall );

private:
	LPDIRECTINPUTDEVICE8	pDIDevice;
};
