// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list.h>
using namespace std;

#include "Sprite.h"

class CMovingSprite;


class CBrick :
	public CSprite  
{
public:
	CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	virtual ~CBrick();

	void BallHits( CBall* pBall, const D3DXVECTOR2 & vSide );

	static void PrepareEnvironment( list<CSprite*>* pListRender, list<CMovingSprite*>* pListFrameMove, BOOL pGameStates[] );
	static list<CSprite*>*			s_pListRender;
	static list<CMovingSprite*>*	s_pListFrameMove;
	static BOOL*					s_pGameStates;
};
