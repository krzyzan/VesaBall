// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "Ball.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, CDeck* ipDeck )
: CSprite( iTexture, D3DXVECTOR2(1, 1), D3DXVECTOR2(7.5f,7.5f), 0, 
		  iPosition, 0XFFFFFFFF )
{
	vDirection = D3DXVECTOR2(0.0001f*(rand()%1000-500),0.0001f*(rand()%1000-500));
	pDeck = ipDeck;
}

CBall::~CBall()
{

}

void CBall::FrameMove()
{
	if (vPosition.x > 1024-16) vDirection.x *= -1;
	if (vPosition.y > 768-16-16 ) 
		if (vPosition.x > pDeck->vPosition.x-16 && vPosition.x < pDeck->vPosition.x+128 ) 
			vDirection.y *= -1;
		else
            bDeleteMe = TRUE;	

	if (vPosition.x < 0) vDirection.x *= -1;
	if (vPosition.y < 0) vDirection.y *= -1;

	vPosition += vDirection;
}