// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "EffectSprite.h"			//TODO: TMP

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrick::CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size, list<CSprite*>* ListRender, list<CMovingSprite*>* ListFrameMove )
: CSprite( Texture, Size, 0, Position, (DWORD)(rand()%192+64) | (DWORD)(rand()%192+64)*0x000100 | (DWORD)(rand()%192+64)*0x010000 | 0xFF000000 )
{
	pListRender = ListRender;
	pListFrameMove = ListFrameMove;
}

CBrick::~CBrick()
{
	CEffectSprite* pEffectSprite = new CEffectSprite( pTexture, vSize, vPosition, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0.25f, 0xFFFFFFFF );
	pListFrameMove->push_back( pEffectSprite );
	pListRender->push_back( pEffectSprite );
}

void CBrick::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide )
{
	//TODO: kod jest szybki ale nawet ja ju¿ go nie rozumiem :(
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
	bDeleteMe = TRUE;
};