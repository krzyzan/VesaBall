// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ball.h"
#include "GameEngine.h"
#include "EffectSprite.h"
#include "Bonus.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

list<CMovingSprite*>*	CBall::s_pListFrameMove;
list<CObject*>*			CBall::s_pListRender;
LPDIRECT3DTEXTURE8		CBall::s_pSparkTexture;


CBall::CBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
	: CMovingSprite( Texture, D3DXVECTOR2(1.0f/64, 1.0f/64), 0, Position, Speed, D3DXVECTOR2(0, 0), 0xFFFFFFFF )
{
}

CBall::~CBall()
{
}

void CBall::PrepareEnvironment( list<CObject*>* pListRender, list<CMovingSprite*>* pListFrameMove, LPDIRECT3DTEXTURE8 pSparkTexture)
{
	s_pListRender = pListRender;
	s_pListFrameMove = pListFrameMove;
	s_pSparkTexture = pSparkTexture;
}

HRESULT CBall::FrameMove( FLOAT fElapsedTime )
{
	CMovingSprite::FrameMove( fElapsedTime );

	// kasuj gdy wyjdzie za ekran
	if (vPosition.y > BOARD_B + vSize.y/2 ) {
		bDeleteMe = TRUE;	
		return S_OK;
	}

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

	if (vPosition.y + vSize.y/2 > BOARD_B) {
		vPosition.y = 2*BOARD_B - vPosition.y - vSize.y;
		vSpeed.y *= -1;
	}


	return S_OK;
}

// Iskry przy odbiciu
void CBall::StrikeSparkles( const D3DXVECTOR2 & vSide )
{
	D3DXVECTOR2 vSparkSize		= D3DXVECTOR2(1.0f/256, 1.0f/256);
	D3DXVECTOR2 vSparkPosition	= vPosition + vSide;
	D3DXVECTOR2 vSparkGravity	= D3DXVECTOR2( 0.0f, 0.4f );
	for (int i=0; i<8; i++) {
			FLOAT fSparkDuration = frand(0.4f, 1.0f);
			D3DXVECTOR2 vSparkSpeed = D3DXVECTOR2( frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) )/10 + vSpeed/4;
			CEffectSprite* pEffectSprite = new CEffectSprite( s_pSparkTexture, vSparkSize, 
				vSparkPosition, vSparkSpeed, vSparkGravity, fSparkDuration, 0xFFFFFFFF );
			s_pListFrameMove->push_back( pEffectSprite );
			s_pListRender->push_back( pEffectSprite );
	}
}
