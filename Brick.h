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
	static const DUR_MAX = 3;
	static const BYTE TYPE_MAX = 5;

	struct STypeDesc
	{
		BYTE				type;
		DWORD				dur;
		LPDIRECT3DTEXTURE8	pTexture[DUR_MAX];
	};

	CBrick( STypeDesc* Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	virtual ~CBrick();

	void ReflectBall( CBall* pBall, const D3DXVECTOR2 & vSide );
	void SetHitCounter( DWORD HitCounter );
	CEffectSprite* CreateEffect();

	DWORD dwHitCounter;

	STypeDesc* pTypeDesc;
	static STypeDesc s_TypeDesc[TYPE_MAX];
};
