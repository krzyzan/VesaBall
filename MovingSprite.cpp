#include "stdafx.h"
#include "movingsprite.h"
#include "level.h"

CMovingSprite::CMovingSprite( CLevel* Level, LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Blending )
	: CSprite( Level, Texture, Size, Rotation, Position, Blending )
{
	pLevel->listFrameMove.push_back( this );

	vSpeed = Speed;
	vAccel = Accel;
}

CMovingSprite::~CMovingSprite(void)
{
}
