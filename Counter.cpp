#include "stdafx.h"
#include "counter.h"


CCounter::CCounter( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, DWORD DigitHeight, DWORD NumDigits )
	: CMovingSprite( Texture, Size, 0, Position, D3DXVECTOR2(), D3DXVECTOR2(), 0xFFFFFFFF )
{
	dwNumRollers = NumDigits;
	dwDigitHeight = DigitHeight;

	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling.y *= sd.Height/dwDigitHeight;
	vScaling.x /= dwNumRollers;
	
	pRoller	= new float[ dwNumRollers ];
	pRollerDest = new DWORD[ dwNumRollers ];

	Reset();
}

CCounter::~CCounter()
{
	delete [] pRoller;
}

void CCounter::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2);
	for (int r=dwNumRollers-1; r>=0; r--) {
		CONST RECT SrcRect = {0, ((LONG)pRoller[r]%10)*dwDigitHeight, 64, ((LONG)pRoller[r]%10 + 1)*dwDigitHeight };
		pSprite->Draw( pTexture, &SrcRect, &vScaling, &vRotationCenter, fRotation, &(Position * RES_X), dwBlending );
		Position.x += vSize.x/dwNumRollers;
	}
}

HRESULT CCounter::FrameMove( FLOAT fElapsedTime )
{
	for (DWORD r=0; r<dwNumRollers; r++)
		pRoller[r] -= (pRoller[r] - pRollerDest[r]) * fElapsedTime * 5;

	return S_OK;
}

void CCounter::Reset()
{
	dwScore = 0;

	ZeroMemory( pRoller, sizeof(pRoller[0])*dwNumRollers );
	ZeroMemory( pRollerDest, sizeof(pRoller[0])*dwNumRollers );
}

void CCounter::Increment( DWORD Delta )
{
	dwScore += Delta;
	DWORD dwScoreDest = dwScore;
	for (DWORD r=0; r<dwNumRollers; r++) {
		pRollerDest[r] = dwScoreDest;
		dwScoreDest /= 10;
	}
}