// Deck.cpp: implementation of the CDeck class.
//
//////////////////////////////////////////////////////////////////////

#include "Deck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeck::CDeck( LPDIRECT3DTEXTURE8 iTexture, LPDIRECTINPUTDEVICE8 ipDIDevice )
 : CSprite( iTexture, D3DXVECTOR2(1,1), D3DXVECTOR2(0,0), 0, D3DXVECTOR2(0.5f, DECK_Y), 0xFFFFFFFF)
{
	pDIDevice = ipDIDevice;
}

CDeck::~CDeck()
{

}

void CDeck::FrameMove( FLOAT fElapsedTime )
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) return;
    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	vPosition.x	+= (FLOAT)dims2.lX * 1.5f / RES_X;

	if (vPosition.x < 0.0f + vSize.x/2) vPosition.x = 0.0f + vSize.x/2;
	if (vPosition.x > 1.0f - vSize.x/2) vPosition.x = 1.0f - vSize.x/2;
}