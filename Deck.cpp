// Deck.cpp: implementation of the CDeck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Deck.h"
#include "Ball.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeck::CDeck( LPDIRECT3DTEXTURE8 iTexture, LPDIRECTINPUTDEVICE8 ipDIDevice)
 : CSprite( iTexture, D3DXVECTOR2(1.0f/8, 1.0f/64), 0, D3DXVECTOR2(0.5f, 0.75f-0.5f/64), 0xFFFFFFFF)
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

void CDeck::Collide( list<CBall*>* pListBall ) 
{
	list<CBall*>::iterator iBall;
	for (iBall = pListBall->begin(); iBall != pListBall->end(); iBall++)
		(*iBall)->Bounce( this );
}