// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "Bonus.h"
#include "EffectSprite.h"			//TODO: TMP

list<CSprite*>*			CBrick::s_pListRender;
list<CMovingSprite*>*	CBrick::s_pListFrameMove;
BOOL*					CBrick::s_pGameStates;

CBrick::CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
	: CSprite( Texture, Size, 0, Position, D3DXCOLOR( frand(0.5f,1.0f), frand(0.5f,1.0f), frand(0.5f,1.0f), 1.0f) )
{
}


CBrick::~CBrick()
{
	CEffectSprite* pEffectSprite = new CEffectSprite( pTexture, vSize, vPosition, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0.25f, 0xFFFFFFFF );
	s_pListFrameMove->push_front( pEffectSprite );
	s_pListRender->push_front( pEffectSprite );
}


void CBrick::PrepareEnvironment( list<CSprite*>* pListRender, list<CMovingSprite*>* pListFrameMove, BOOL* pGameStates )
{
	s_pListRender = pListRender;
	s_pListFrameMove = pListFrameMove;
	s_pGameStates = pGameStates;
}


void CBrick::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide )
{
	if (frand(0,1) < BONUS_PROBABILITY) {
		CBonus* pBonus = new CBonus( static_cast<CBonus::TYPE>(rand()%CBonus::MAX_TYPE), pBall->vPosition, pBall->vSpeed/2 );
		s_pListRender->push_back( pBonus );
		s_pListFrameMove->push_back( pBonus );
	}

	if (!s_pGameStates[CBonus::GhostBall] ) {
		if (vSide.y){
			pBall->vSpeed.y *= -1;
			pBall->vPosition.y = 2*(vPosition.y + vSide.y) - pBall->vPosition.y;
			pBall->vPosition.y += (vSide.y > 0) ? vSize.y : (-vSize.y);
		}

		if (vSide.x) {
			pBall->vSpeed.x *= -1;
			pBall->vPosition.x = 2*(vPosition.x + vSide.x) - pBall->vPosition.x;
			pBall->vPosition.x += (vSide.x > 0) ? vSize.x : (-vSize.x);
		}
		
		pBall->StrikeSparkles( vSide );
	}

	bDeleteMe = TRUE;
};