#pragma once
#include "MovingSprite.h"


class CEffectSprite :
	public CMovingSprite
{
public:
	CEffectSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, 
		float Duration, DWORD Blending );
	virtual ~CEffectSprite(void);

	void FrameMove( float fElapsedTime );
	bool Expired() const 
		{ return fRemaining < 0; }

private:
	float fRemaining;
	float fDuration;
};

