#include "StdAfx.h"
#include "SpriteEffect.h"

CSpriteEffect::CSpriteEffect(LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size,
							 const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel,
							 DWORD Color, float Duration)
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
	D3DXCOLOR dxBlending(dwColor);
	dxBlending.a = fRemaining / fDuration;
	dwColor = dxBlending;
}

bool CSpriteEffect::HasExpired() const
{
	return fRemaining < 0;
}
