// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "Ball.h"
#include "Deck.h"
#include "SparkEffect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, D3DXVECTOR2 iDirection, list<CSprite*>* ipListRender, LPDIRECT3DTEXTURE8 iSparkTexture )
: CSprite( iTexture, D3DXVECTOR2(1.0f/64, 1.0f/64), 0, iPosition, 0xFFFFFFFF )
{
	pListRender = ipListRender;
	vDirection = iDirection;
	pSparkTexture = iSparkTexture;
}

CBall::~CBall()
{

}

void CBall::FrameMove( FLOAT fElapsedTime )
{
	vOldPosition = vPosition;
	vPosition += vDirection*fElapsedTime;

	fRotation += 10.0f*fElapsedTime;

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
}

void CBall::Bounce( CSprite* pDeck )
{
    D3DXVECTOR2 vSparkPosition(0.0f, 0.0f);

	if (fabs(vOldPosition.x - pDeck->vPosition.x) < vSize.x/2 + pDeck->vSize.x/2 &&
		fabs(vPosition.y - pDeck->vPosition.y) < vSize.y/2 + pDeck->vSize.y/2 )
	{
		if (vDirection.y > 0) {
			vSparkPosition = vPosition + D3DXVECTOR2( 0, vSize.y/2 );
			vPosition.y = 2*(pDeck->vPosition.y - pDeck->vSize.y/2) - vPosition.y - vSize.y;
		} 
		else {
			vSparkPosition = vPosition - D3DXVECTOR2( 0, vSize.y/2 );
			vPosition.y = 2*(pDeck->vPosition.y + pDeck->vSize.y/2) - vPosition.y + vSize.y;
		}
		vDirection.y *= -1;

		//kod na krzywe odbicie od deski
		FLOAT speed = D3DXVec2Length( &vDirection );
		vDirection.x += (vPosition.x - pDeck->vPosition.x) * 5;
		vDirection *= speed / D3DXVec2Length( &vDirection );
	}
	else
	if (fabs(vOldPosition.y - pDeck->vPosition.y) < vSize.y/2 + pDeck->vSize.y/2 &&
		fabs(vPosition.x - pDeck->vPosition.x) < vSize.x/2 + pDeck->vSize.x/2 )
	{
		if (vDirection.x > 0) {
			vSparkPosition = vPosition + D3DXVECTOR2( vSize.x/2, 0 );
			vPosition.x = 2*(pDeck->vPosition.x - pDeck->vSize.x/2) - vPosition.x - vSize.x;
		}
		else {
			vSparkPosition = vPosition - D3DXVECTOR2( vSize.x/2, 0 );
			vPosition.x = 2*(pDeck->vPosition.x + pDeck->vSize.x/2) - vPosition.x + vSize.x;
		}
		vDirection.x *= -1;
	}
    
	// Iskry przy odbiciu
	if ( vSparkPosition != D3DXVECTOR2(0.0f, 0.0f) ) {
		for ( int i=0; i<8; i++ ) {
			D3DXVECTOR2 vSparkDirection = vDirection/4 + D3DXVECTOR2(0.001f*(rand()%256-128), 0.001f*(rand()%256-128) );
			CSparkEffect* se = new CSparkEffect( pSparkTexture, vSparkPosition, vSparkDirection, 0.1f*(rand()%8+8) );
			pListRender->push_back( se );
		}
	}
}