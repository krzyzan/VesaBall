// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

class CBall;
class CEffectSprite;


class CBrick :
	public CSprite  
{
public:
	CBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	virtual ~CBrick();

	void ReflectBall( CBall* pBall, const D3DXVECTOR2 & vSide );
	CEffectSprite* CreateEffect();
};
