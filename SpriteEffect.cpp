#include "StdAfx.h"
#include "SpriteEffect.h"

CSpriteEffect::CSpriteEffect(SDL_Texture* Texture, const Vec2 & Size,
							 const Vec2 & Position, const Vec2 & Speed, const Vec2 & Accel,
							 Color Color, float Duration)
	: CSpriteMoving(Texture, Size, 0, Position, Speed, Accel, Color)
{
	fDuration = fRemaining = Duration;
}

CSpriteEffect::~CSpriteEffect()
{
}

void CSpriteEffect::FrameMove(float fElapsedTime)
{
	CSpriteMoving::FrameMove(fElapsedTime);

	fRemaining -= fElapsedTime;
	dwColor = ColorWithAlpha(dwColor, fRemaining / fDuration);
}

bool CSpriteEffect::HasExpired() const
{
	return fRemaining < 0;
}
