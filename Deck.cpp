// Deck.cpp: implementation of the CDeck class.
//
//////////////////////////////////////////////////////////////////////

#include "Deck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeck::CDeck( LPDIRECT3DTEXTURE8 iTexture, LPDIRECTINPUTDEVICE8 ipDIDevice )
 : CSprite( iTexture, D3DXVECTOR2(1,1), D3DXVECTOR2(0,0), 0, D3DXVECTOR2(RES_X/2, RES_Y-DECK_UP), 0xFFFFFFFF)
{
	pDIDevice = ipDIDevice;
}

CDeck::~CDeck()
{

}

void CDeck::FrameMove()
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) return;
    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	vPosition.x	+= dims2.lX*2;

	if (vPosition.x < 0		+ vTexSize.x/2) vPosition.x = 0		+ vTexSize.x/2;
	if (vPosition.x > RES_X	- vTexSize.x/2) vPosition.x = RES_X	- vTexSize.x/2;
}