// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list.h>
#include "Sprite.h"

#define BRICK_X		20
#define BRICK_Y		40

using namespace std;

class CMovingSprite;		//TMP

class CBrick : public CSprite  
{
public:
	CBrick( LPDIRECT3DTEXTURE8 Texture,  const D3DXVECTOR2 & Position, list<CSprite*>* ListRender, list<CMovingSprite*>* ListFrameMove );
	virtual ~CBrick();

	void Collide( list<CBall*>* pListBall, BOOL & bThruBrick );

	list<CSprite*>*			pListRender;
	list<CMovingSprite*>*	pListFrameMove;
};
