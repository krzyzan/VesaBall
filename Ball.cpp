// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "Ball.h"
#include <Dxerr8.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, CDeck* ipDeck )
: CSprite( iTexture, D3DXVECTOR2(0.2f, 0.2f), D3DXVECTOR2(7.5f,7.5f), 0, 
		  iPosition, 0XFFFFFFFF )
{
	vDirection = D3DXVECTOR2(0.1f*(rand()%2000-1000),0.1f*(rand()%2000-1000));
	pDeck = ipDeck;
	Timer.Start();
}

CBall::~CBall()
{

}

void CBall::FrameMove()
{

	
	D3DXVECTOR2 vNewPosition = vPosition + vDirection*Timer.GetElapsedTime();

	if (vNewPosition.x - vTexSize.x/2 < 0) {
		vDirection.x *= -1;
		vNewPosition.x = 0 - vNewPosition.x + vTexSize.x;
	}

	if (vNewPosition.y - vTexSize.y/2 < 0) {
		vDirection.y *= -1;
		vNewPosition.y = 0 - vNewPosition.y + vTexSize.y;
	}
	
	
	if (vNewPosition.x + vTexSize.x/2 > RES_X) {
		vDirection.x *= -1;
		vNewPosition.x = 2*RES_X - vNewPosition.x - vTexSize.x;
	}

	if (vNewPosition.y + vTexSize.y/2 > RES_Y) {
		vDirection.y *= -1;
		vNewPosition.y = 2*RES_Y - vNewPosition.y - vTexSize.y;
	}
	
	/*
	//odbicia od œcian
	if (vPosition.x - vTexSize.x/2 < 0)		vDirection.x *= -1;
	if (vPosition.y - vTexSize.y/2 < 0)		vDirection.y *= -1;
	if (vPosition.x + vTexSize.x/2 > RES_X)	vDirection.x *= -1;

	//odbicie od deski
	if (vPosition.x + vTexSize.x/2 > pDeck->vPosition.x - pDeck->vTexSize.x/2 && 
		vPosition.x - vTexSize.x/2 < pDeck->vPosition.x + pDeck->vTexSize.x/2 )
		if (vPosition.y + vTexSize.y/2 > pDeck->vPosition.y - pDeck->vTexSize.y/2 &&
			vPosition.y + vTexSize.y/2 < pDeck->vPosition.y - pDeck->vTexSize.y/2 + 1)
		{
			vDirection.y *= -1;
			//FLOAT speed = D3DXVec2Length( &vDirection );
			//vDirection.x += (vPosition.x - pDeck->vPosition.x) / 10000;
			//FLOAT newspeed = D3DXVec2Length( &vDirection );
			//vDirection.x *= speed / newspeed;
			//vDirection.y *= speed / newspeed;
		}

	if (vPosition.y > RES_Y + vTexSize.y/2 ) 
			bDeleteMe = TRUE;	

	vPosition += vDirection * Timer.GetElapsedTime();
	*/
	vPosition = vNewPosition;
}