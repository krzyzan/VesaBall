// Deck.h: interface for the CDeck class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#define DIRECTINPUT_VERSION  0x0800

#include <dinput.h>
#include "Sprite.h"

class CDeck : public CSprite  
{
public:
	CDeck::CDeck( LPDIRECT3DTEXTURE8 iTexture, LPDIRECTINPUTDEVICE8 ipDIDevice );
	virtual ~CDeck();

	virtual void FrameMove( FLOAT fElapsedTime );

private:
	LPDIRECTINPUTDEVICE8	pDIDevice;
};
