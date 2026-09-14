#include "StdAfx.h"
#include "Counter.h"

const float COUNTER_UPDATE_SPEED = 5;

SDL_Texture* CCounter::spTexture;

CCounter::CCounter(LONG Value, const Vec2 & Size, const Vec2 & Position,
				   POINT DigitPixels, DWORD NumDigits)
	: CSprite(spTexture, Size, 0, Position, 0xFFFFFFFF)
{
	dwNumDigits = NumDigits;
	dwDigitPixels = DigitPixels;

	lValue = Value;
	fValue = (float)Value;
}

CCounter::~CCounter()
{
}

void CCounter::Render(SDL_Renderer* pRenderer) const
{
	Vec2 digitSize(vSize.x / dwNumDigits, vSize.y);
	Vec2 center = vPosition + Vec2(vSize.x / 2 - digitSize.x / 2, 0);

	LONG lDigit = LONG(fValue + 0.5f);
	if (lDigit < 0)
		lDigit = 0;
	for (DWORD r = 0; r < dwNumDigits; r++)
	{
		SDL_Rect SrcRect;
		SrcRect.x = 0;
		SrcRect.y = (lDigit % 10) * dwDigitPixels.y;
		SrcRect.w = dwDigitPixels.x;
		SrcRect.h = dwDigitPixels.y;
		DrawTexture(pRenderer, pTexture, &SrcRect, center, digitSize, fRotation, dwColor, bFlipX, bFlipY);
		center.x -= digitSize.x;
		lDigit /= 10;
	}
}

void CCounter::Update(float fElapsedTime)
{
	fValue -= (fValue - lValue) * fElapsedTime * COUNTER_UPDATE_SPEED;
}
