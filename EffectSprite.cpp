#include "StdAfx.h"
#include "EffectSprite.h"


CEffectSprite::CEffectSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, 
		DWORD Blending, float Duration )
	: CMovingSprite( Texture, Size, 0, Position, Speed, Accel, Blending )
{
	fDuration = fRemaining = Duration;
}

CEffectSprite::~CEffectSprite()
{
}


void CEffectSprite::FrameMove( float fElapsedTime )
{
	CMovingSprite::FrameMove( fElapsedTime );
	
	fRemaining -= fElapsedTime;
	D3DXCOLOR dxBlending( dwBlending );
	dxBlending.a = fRemaining/fDuration;
	dwBlending = dxBlending;
}
