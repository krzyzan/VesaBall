#include "stdafx.h"
#include "bonus.h"

list<CSprite*>*		CBonus::s_pListObst;
LPDIRECT3DTEXTURE8*	CBonus::s_pTextures;

CBonus::CBonus( TYPE Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
	: CMovingSprite( s_pTextures[Type], D3DXVECTOR2(1.0f/32, 1.0f/32), 0, Position, Speed, D3DXVECTOR2(0, 0.25f), 0xFFFFFFFF )
{
	eType = Type;
}

CBonus::~CBonus()
{
}

void CBonus::PrepareEnvironment( list<CSprite*>* pListObst, LPDIRECT3DTEXTURE8* pTextures )
{
	s_pListObst = pListObst;
	s_pTextures = pTextures;
}

HRESULT CBonus::FrameMove( FLOAT fElapsedTime )
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

	// odbicia od przeszkód
	list<CSprite*>::iterator iObst;
	for (iObst = s_pListObst->begin(); iObst != s_pListObst->end(); iObst++) {
		D3DXVECTOR2 vSide = IsColliding( *iObst );
		if (vSide == D3DXVECTOR2(0,0)) 
			continue;
		(*iObst)->BonusHits( this, vSide );
	}

	return S_OK;
}
