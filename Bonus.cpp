#include "StdAfx.h"
#include "Bonus.h"

LPDIRECT3DTEXTURE8*	CBonus::s_pTextures;

CBonus::CBonus( DWORD Type, const LPDIRECT3DTEXTURE8* pBonusTextures, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
	: CMovingSprite( pBonusTextures[Type], D3DXVECTOR2(1.0f/32, 1.0f/32), 0, Position, Speed, D3DXVECTOR2(0, 0.25f), 0xFFFFFFFF )
{
	dwType = Type;
}


CBonus::~CBonus()
{
}


HRESULT CBonus::FrameMove( FLOAT fElapsedTime )
{
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

	return S_OK;
}
