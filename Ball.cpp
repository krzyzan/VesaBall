// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ball.h"
#include "EffectSprite.h"

LPDIRECT3DTEXTURE8 CBall::s_pTexture;
LPDIRECT3DTEXTURE8 CBall::s_pSparkTexture;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall( const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
	: CMovingSprite( s_pTexture, D3DXVECTOR2(BALL_SIZE_AVG, BALL_SIZE_AVG), 0, Position, Speed, D3DXVECTOR2(0, 0), 0xFFFFFFFF )
{
	bCatched	= false;
}


CBall::~CBall()
{
}


HRESULT CBall::FrameMove( FLOAT fElapsedTime )
{
	if (bCatched)
		return S_OK;
		
	CMovingSprite::FrameMove( fElapsedTime );

	// odbicia od œcian
	if (vPosition.x - vSize.x/2 < BOARD_L) {
		vPosition.x = 2*BOARD_L - vPosition.x + vSize.x;
		vSpeed.x *= -1;
	}

	if (vPosition.x + vSize.x/2 > BOARD_R) {
		vPosition.x = 2*BOARD_R - vPosition.x - vSize.x;
		vSpeed.x *= -1;
	}

	if (vPosition.y - vSize.y/2 < BOARD_T) {
		vPosition.y = 2*BOARD_T - vPosition.y + vSize.y;
		vSpeed.y *= -1;
	}

	/*
	if (vPosition.y + vSize.y/2 > BOARD_B) {
		vPosition.y = 2*BOARD_B - vPosition.y - vSize.y;
		vSpeed.y *= -1;
	}
	*/

	return S_OK;
} 

// Iskry przy odbiciu
void CBall::CreateSparkles( const D3DXVECTOR2 & vSide, list<CEffectSprite*>* pListEffect ) const
{
	D3DXVECTOR2 vSparkSize		= D3DXVECTOR2(1.0f/256, 1.0f/256);
	D3DXVECTOR2 vSparkPosition	= vPosition + vSide;
	D3DXVECTOR2 vSparkGravity	= D3DXVECTOR2( 0.0f, 0.2f );
	for (int i=0; i<8; i++) {
		FLOAT fSparkDuration = frand(0.5f, 1.0f);
		D3DXVECTOR2 vSparkSpeed = D3DXVECTOR2( frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) )/10 + vSpeed/8;
		CEffectSprite* pEffectSprite = new CEffectSprite( s_pSparkTexture, vSparkSize, 
			vSparkPosition, vSparkSpeed, vSparkGravity, fSparkDuration, 0xFFFFFFFF );
		pListEffect->push_back( pEffectSprite );
	}
}

void CBall::MultiplySpeed( float fFactor )
{
	float fNewSpeed = D3DXVec2Length( &vSpeed ) * fFactor;
	fNewSpeed = max( fNewSpeed, BALL_SPEED_MIN );
	fNewSpeed = min( fNewSpeed, BALL_SPEED_MAX );
	D3DXVec2Normalize( &vSpeed, &vSpeed );
	vSpeed *= fNewSpeed;
}
