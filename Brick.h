// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;

#include <list.h>
#include "Sprite.h"

class CMovingSprite;

class CBrick :
	public CSprite  
{
public:
	CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size, list<CSprite*>* ListRender, list<CMovingSprite*>* ListFrameMove );
	virtual ~CBrick();

	void BallHits( CBall* pBall, const D3DXVECTOR2 & vSide );

	list<CSprite*>*			pListRender;
	list<CMovingSprite*>*	pListFrameMove;
};
