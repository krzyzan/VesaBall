#pragma once
#include "Sprite.h"

class CMovingSprite : public CSprite
{
public:
	CMovingSprite( CLevel* Level, LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Blending );
	virtual ~CMovingSprite(void);

	D3DXVECTOR2	vOldPosition;
	D3DXVECTOR2	vSpeed;
	D3DXVECTOR2	vAccel;

	virtual void FrameMove( FLOAT fElapsedTime ) = 0;
};
