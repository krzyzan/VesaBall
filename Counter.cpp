#include "StdAfx.h"
#include "Counter.h"

const float COUNTER_UPDATE_SPEED = 5;

LPDIRECT3DTEXTURE8 CCounter::spTexture;

CCounter::CCounter(LONG Value, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position,
				   POINT DigitPixels, DWORD NumDigits)
	: CSprite(spTexture, Size, 0, Position, 0xFFFFFFFF)
{
	dwNumDigits = NumDigits;
	dwDigitPixels = DigitPixels;

	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling.y *= sd.Height / DigitPixels.y;
	vScaling.x /= dwNumDigits;

	lValue = Value;
	fValue = (float)Value;
}

CCounter::~CCounter()
{
}

void CCounter::Render(LPD3DXSPRITE pSprite) const
{
	D3DXVECTOR2 Position = vPosition - vSize / 2 + D3DXVECTOR2(vSize.x, 0);
	Position *= (float)RES_X;
	LONG lDigit = LONG(fValue + 0.5f);
	if (lDigit < 0)
		lDigit = 0;
	for (DWORD r = 0; r < dwNumDigits; r++)
	{
		Position.x -= vSize.x / dwNumDigits * (float)RES_X;
		CONST RECT SrcRect = {0, (lDigit % 10) * dwDigitPixels.y, dwDigitPixels.x, (lDigit % 10 + 1) * dwDigitPixels.y};
		pSprite->Draw(pTexture, &SrcRect, &vScaling, &vRotationCenter, fRotation, &Position, dwColor);
		lDigit /= 10;
	}
}

void CCounter::Update(float fElapsedTime)
{
	fValue -= (fValue - lValue) * fElapsedTime * COUNTER_UPDATE_SPEED;
}
