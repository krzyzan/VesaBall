#include "StdAfx.h"
#include "Counter.h"

LPDIRECT3DTEXTURE8 CCounter::s_pTexture;

CCounter::CCounter( LONG Value, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position, 
		DWORD DigitHeight, DWORD NumDigits )
	: CSprite( s_pTexture, Size, 0, Position, 0xFFFFFFFF )
{
	dwNumDigits = NumDigits;
	dwDigitHeight = DigitHeight;

	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling.y *= sd.Height/dwDigitHeight;
	vScaling.x /= dwNumDigits;
	
	lValue = Value;
	fValue = (float)Value;
}


void CCounter::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = vPosition - vSize/2 + D3DXVECTOR2(vSize.x,0);
	LONG lDigit = LONG(fValue + 0.5f);
	for (DWORD r=0; r<dwNumDigits; r++) {
		Position.x -= vSize.x/dwNumDigits;
		CONST RECT SrcRect = {0, (lDigit%10)*dwDigitHeight, 64, (lDigit%10+1)*dwDigitHeight };
		pSprite->Draw( pTexture, &SrcRect, &vScaling, &vRotationCenter, fRotation, &(Position * RES_X), dwBlending );
		lDigit /= 10;
	}
}

void CCounter::Update( float fElapsedTime )
{
	fValue -= (fValue - lValue)  * fElapsedTime * 5;
}
