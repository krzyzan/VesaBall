// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ball.h"
#include "Level.h"
#include "EffectSprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, list<CSprite*>* ListObst, 
		list<CSprite*>* ListRender, list<CMovingSprite*>* ListFrameMove, LPDIRECT3DTEXTURE8 SparkTexture )
	: CMovingSprite( Texture, D3DXVECTOR2(1.0f/64, 1.0f/64), 0, Position, Speed, D3DXVECTOR2(0, 0), 0xFFFFFFFF )
{
	pListObst = ListObst;
	pListRender = ListRender;
	pListFrameMove = ListFrameMove;
	pSparkTexture = SparkTexture;
}

CBall::~CBall()
{

}

D3DXVECTOR2 CBall::IsColliding( CSprite* pSprite )
{
	if (fabs(vOldPosition.x - pSprite->vPosition.x) < vSize.x/2 + pSprite->vSize.x/2 &&
			fabs(vPosition.y - pSprite->vPosition.y) < vSize.y/2 + pSprite->vSize.y/2 )
		return (vSpeed.y > 0) ? D3DXVECTOR2( 0, -vSize.y/2 ) : D3DXVECTOR2( 0, vSize.y/2 );

	if (fabs(vOldPosition.y - pSprite->vPosition.y) < vSize.y/2 + pSprite->vSize.y/2 &&
			fabs(vPosition.x - pSprite->vPosition.x) < vSize.x/2 + pSprite->vSize.x/2 )
		return (vSpeed.x > 0) ? D3DXVECTOR2( -vSize.x/2, 0 ) : D3DXVECTOR2( vSize.x/2, 0 );

	return D3DXVECTOR2(0,0);
}

HRESULT CBall::FrameMove( FLOAT fElapsedTime )
{
	vOldPosition = vPosition;
	vPosition += vSpeed*fElapsedTime;

	if (vPosition.y > 0.75f + vSize.y/2 ) {
		bDeleteMe = TRUE;	
		return S_OK;
	}

	//odbicia od œcian
	if (vPosition.x - vSize.x/2 < 0.0f) {
		vPosition.x = 2*0.00f - vPosition.x + vSize.x;
		vSpeed.x *= -1;
	}

	if (vPosition.y - vSize.y/2 < 0.0f) {
		vPosition.y = 2*0.00f - vPosition.y + vSize.y;
		vSpeed.y *= -1;
	}

	if (vPosition.x + vSize.x/2 > 1.0f) {
		vPosition.x = 2*1.00f - vPosition.x - vSize.x;
		vSpeed.x *= -1;
	}

	/*
	if (vPosition.y + vSize.y/2 > 0.75f) {
		vPosition.y = 2*0.75f - vPosition.y - vSize.y;
		vSpeed.y *= -1;
	}
	*/

	// odbicia od przeszkód
	list<CSprite*>::iterator iObst;
	for (iObst = pListObst->begin(); iObst != pListObst->end(); iObst++) {
		D3DXVECTOR2 vSide = IsColliding( *iObst );
		if (vSide == D3DXVECTOR2(0,0)) continue;
		(*iObst)->BallHits( this, vSide );
	}

	return S_OK;
}

// Iskry przy odbiciu
void CBall::StrikeSparkles( const D3DXVECTOR2 & vPositionFromCenter )
{
	D3DXVECTOR2 vSparkSize		= D3DXVECTOR2(1.0f/256, 1.0f/256);
	D3DXVECTOR2 vSparkPosition	= vPositionFromCenter + vPosition;
	D3DXVECTOR2 vSparkGravity	= D3DXVECTOR2( 0.0f, 0.4f );
	for (int i=0; i<8; i++) {
			FLOAT fSparkDuration	= frand(0.4f, 1.0f);
			D3DXVECTOR2 vSparkSpeed = D3DXVECTOR2( frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) )/10 + vSpeed/4;
			CEffectSprite* pEffectSprite = new CEffectSprite( pSparkTexture, vSparkSize, vSparkPosition, vSparkSpeed, vSparkGravity, fSparkDuration, 0xFFFFFFFF );
			pListFrameMove->push_back( pEffectSprite );
			pListRender->push_back( pEffectSprite );
	}
}
