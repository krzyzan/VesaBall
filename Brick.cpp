// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Level.h"
#include "EffectSprite.h"			//TMP

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrick::CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, list<CSprite*>* ListRender, list<CMovingSprite*>* ListFrameMove )
: CSprite( Texture, D3DXVECTOR2(1.0f/BRICK_X, 1.0f/BRICK_Y), 0, Position, (DWORD)(rand()%192+64) | (DWORD)(rand()%192+64)*0x000100 | (DWORD)(rand()%192+64)*0x010000 | 0xFF000000 )
{
	pListRender = ListRender;
	pListFrameMove = ListFrameMove;
}

CBrick::~CBrick()
{
	CEffectSprite* pEffectSprite = new CEffectSprite( pTexture, vSize, vPosition, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0.25f, dwBlending );
	pListFrameMove->push_back( pEffectSprite );
	pListRender->push_back( pEffectSprite );
}

void CBrick::Collide( list<CBall*>* pListBall, BOOL & bThruBrick )
{
	list<CBall*>::iterator iBall;
	for (iBall = pListBall->begin(); iBall != pListBall->end(); iBall++) {
		CBall* pBall = (*iBall);

		if (fabs(pBall->vOldPosition.x - vPosition.x) < pBall->vSize.x/2 + vSize.x/2 &&
			fabs(pBall->vPosition.y - vPosition.y) < pBall->vSize.y/2 + vSize.y/2 )
		{
			bDeleteMe = TRUE;
			if (!bThruBrick) {
				if (pBall->vSpeed.y > 0) {
					pBall->vSpeed.y *= -1;
					pBall->ThrowSparkles( D3DXVECTOR2( 0,  pBall->vSize.y/2 ) );
					pBall->vPosition.y = 2*(vPosition.y - vSize.y/2) - pBall->vPosition.y - pBall->vSize.y;
				} 
				else {
					pBall->vSpeed.y *= -1;
					pBall->ThrowSparkles( D3DXVECTOR2( 0, -pBall->vSize.y/2 ) );
					pBall->vPosition.y = 2*(vPosition.y + vSize.y/2) - pBall->vPosition.y + pBall->vSize.y;
				}
			}
		}
		else
		if (fabs(pBall->vOldPosition.y - vPosition.y) < pBall->vSize.y/2 + vSize.y/2 &&
			fabs(pBall->vPosition.x - vPosition.x) < pBall->vSize.x/2 + vSize.x/2 )
		{
			bDeleteMe = TRUE;
			if (!bThruBrick) {
				if (pBall->vSpeed.x > 0) {
					pBall->vSpeed.x *= -1;
					pBall->ThrowSparkles( D3DXVECTOR2( 0,  pBall->vSize.x/2 ) );
					pBall->vPosition.x = 2*(vPosition.x - vSize.x/2) - pBall->vPosition.x - pBall->vSize.x;
				} 
				else {
					pBall->vSpeed.x *= -1;
					pBall->ThrowSparkles( D3DXVECTOR2( 0, -pBall->vSize.x/2 ) );
					pBall->vPosition.x = 2*(vPosition.x + vSize.x/2) - pBall->vPosition.x + pBall->vSize.x;
				}
			}
		}
	}
};