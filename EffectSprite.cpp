#include "stdafx.h"
#include "effectsprite.h"
#include "level.h"

CEffectSprite::CEffectSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, 
		FLOAT Duration, DWORD Blending )
	: CMovingSprite( Texture, Size, 0, Position, Speed, Accel, Blending )
{
	fDuration = fRemaining = Duration;
}

CEffectSprite::~CEffectSprite()
{
}

void CEffectSprite::FrameMove( FLOAT fElapsedTime )
{
	vSpeed += vAccel * fElapsedTime;
	vPosition += vSpeed * fElapsedTime;

	dwBlending = 0x00FFFFFF;
	dwBlending += ((DWORD)(fRemaining/fDuration * 0xFF) << 24);

	fRemaining -= fElapsedTime;
    if (fRemaining < 0) 
		bDeleteMe = TRUE;
}
