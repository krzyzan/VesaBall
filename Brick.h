// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"

class CBall;
class CEffectSprite;

const BYTE BRICK_TYPES = 4;

struct BrickTypeDesc
{
	DWORD		dwDurability;
	DWORD		dwBlending;
};

const BrickTypeDesc brickDescTable[BRICK_TYPES] = { 
	{1,			0xFFFFCF00}, 
	{1,			0xFFCFFF00}, 
	{1,			0xFF00CFFF},
	{0xFFFFFFFF,0xFFFFFFFF}, 
};

class CBrick :
	public CSprite  
{
public:
	CBrick( BYTE Type, LPDIRECT3DTEXTURE8* Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	virtual ~CBrick();

	void ReflectBall( CBall* pBall, const D3DXVECTOR2 & vSide );
	CEffectSprite* CreateEffect();

	BYTE dwType;			//TODO
	DWORD dwDurability;
};
