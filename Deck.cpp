// Deck.cpp: implementation of the CDeck class.
//
//////////////////////////////////////////////////////////////////////

#include "Deck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeck::CDeck( LPDIRECT3DTEXTURE8 iTexture, LPDIRECTINPUTDEVICE8 ipDIDevice )
 : CSprite( iTexture, D3DXVECTOR2(1,1), D3DXVECTOR2(0,0), 0, D3DXVECTOR2(512-128/2, 768-16), 0xFFFFFFFF)
{
	pDIDevice = ipDIDevice;
}

CDeck::~CDeck()
{

}

void CDeck::FrameMove()
{
	DIMOUSESTATE2 dims2;

    if( NULL == pDIDevice ) return;
    ZeroMemory( &dims2, sizeof(dims2) );
  	if( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
	{
		pDIDevice->Acquire();
	};
	vPosition.x	+= dims2.lX*2;
	if (vPosition.x>1024-128) vPosition.x=1024-128;
	if (vPosition.x<0) vPosition.x=0;
}