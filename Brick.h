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
	CBrick( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition );
	virtual ~CBrick();

	void FrameMove( FLOAT fElapsedTime );
	void Collide( list<CBall*>* pListBall );
};
