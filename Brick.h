// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list.h>
using namespace std;

#include "Object.h"

class CBall;
class CMovingSprite;


class CBrick :
	public CObject  
{
public:
	CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	virtual ~CBrick();

	void ReflectBall( CBall* pBall, const D3DXVECTOR2 & vSide );

	static void PrepareEnvironment( list<CObject*>* pListRender, list<CMovingSprite*>* pListFrameMove );
	static list<CObject*>*			s_pListRender;
	static list<CMovingSprite*>*	s_pListFrameMove;
};
