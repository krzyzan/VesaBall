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
		  iPosition, 0xFFFFFFFF )
{
	vDirection = D3DXVECTOR2(0.0001f*(rand()%2000-1000),0.0001f*(rand()%2000-1000));
	pDeck = ipDeck;
}

CBall::~CBall()
{

}

void CBall::FrameMove( FLOAT fElapsedTime )
{
	D3DXVECTOR2 vOldPosition = vPosition;
	vPosition += vDirection*fElapsedTime;

	if (vPosition.y > 0.75f + vSize.y/2 ) {
		bDeleteMe = TRUE;	
		return;
	}

	//odbicia od œcian
	if (vPosition.x - vSize.x/2 < 0.0f) {
		vPosition.x = 2*0.00f - vPosition.x + vSize.x;
		vDirection.x *= -1;
	}

	if (vPosition.y - vSize.y/2 < 0.0f) {
		vPosition.y = 2*0.00f - vPosition.y + vSize.y;
		vDirection.y *= -1;
	}

	if (vPosition.x + vSize.x/2 > 1.0f) {
		vPosition.x = 2*1.00f - vPosition.x - vSize.x;
		vDirection.x *= -1;
	}

	/*
	if (vPosition.y + vSize.y/2 > 0.75f) {
		vPosition.y = 2*0.75f - vPosition.y - vSize.y;
		vDirection.y *= -1;
	}
	*/

	if (abs(vOldPosition.x - pDeck->vPosition.x) < vSize.x/2 + pDeck->vSize.x/2 &&
		abs(vPosition.y - pDeck->vPosition.y) < vSize.y/2 + pDeck->vSize.y/2 )
	{
		if (vDirection.y > 0) {
			vPosition.y = 2*(pDeck->vPosition.y - pDeck->vSize.y/2) - vPosition.y - vSize.y;
			vDirection.y *= -1;
		} 
		else {
			vPosition.y = 2*(pDeck->vPosition.y + pDeck->vSize.y/2) - vPosition.y + vSize.y;
			vDirection.y *= -1;
		}
		return;
	}

	if (abs(vOldPosition.y - pDeck->vPosition.y) < vSize.y/2 + pDeck->vSize.y/2 &&
		abs(vPosition.x - pDeck->vPosition.x) < vSize.x/2 + pDeck->vSize.x/2 )
	{
		if (vDirection.x > 0) {
			vPosition.x = 2*(pDeck->vPosition.x - pDeck->vSize.x/2) - vPosition.x - vSize.x;
			vDirection.x *= -1;
		}
		else {
			vPosition.x = 2*(pDeck->vPosition.x + pDeck->vSize.x/2) - vPosition.x + vSize.x;
			vDirection.x *= -1;
		}
		return;
	}

	//kod na krzywe odbicie od deski
	
	//vDirection.y *= -1;
	//FLOAT speed = D3DXVec2Length( &vDirection );
	//vDirection.x += (vPosition.x - pDeck->vPosition.x) / 10000;
	//FLOAT newspeed = D3DXVec2Length( &vDirection );
	//vDirection.x *= speed / newspeed;
	//vDirection.y *= speed / newspeed;
}