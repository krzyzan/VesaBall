#pragma once
#include "MovingSprite.h"

class CEffectSprite : public CMovingSprite
{
public:
	CEffectSprite( CLevel* Level, LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Acceleration, 
		FLOAT Duration, DWORD Blending );
	virtual ~CEffectSprite(void);

	void FrameMove( FLOAT fElapsedTime );

protected:
	FLOAT fRemaining;
	FLOAT fDuration;
	FLOAT fGravity;
};

