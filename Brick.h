// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

#define BRICK_X		20
#define BRICK_Y		40

class CBrick : public CSprite  
{
public:
	CBrick( CLevel* Level, LPDIRECT3DTEXTURE8 Texture,  const D3DXVECTOR2 & Position );
	virtual ~CBrick();

	void Collide( list<CBall*>* pListBall );
};
