// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "EffectSprite.h"

CBrick::SType CBrick::s_Type[BRICK_TYPE_MAX] = { 
	{  1,	0,	1}, 
	{  2,	0,	1}, 
	{  3,	1,	1},
	{  5,	0,	1},
	{  8,	0,	1},
	{ 13,	0,	1}, 
	{ 21,	0,	1}, 
	{ 34,	0,	1},
	{ 55,	0,	1},
	{ 89,	0,	1},
	{144,	0,	DUR_MAX}, 
	{233,	0,	3},
	{377,	0,	2},
};

LPDIRECT3DTEXTURE8 CBrick::s_pTexture[BRICK_TYPE_MAX][DUR_MAX];

CBrick::CBrick( DWORD Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
: CSprite( s_pTexture[Type][0], Size, 0, Position, 0xFFFFFFFF )
{
	dwType		= Type;
	dwHitCount	= 0;
}


CEffectSprite* CBrick::CreateBlendEffect() const
{
	return new CEffectSprite( pTexture, vSize, vPosition, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0xFFFFFFFF, 0.25f );
}


void CBrick::Hit()
{
	if ( IsDestructible() ) {
		dwHitCount++;
		if ( !IsDestroyed() )
			pTexture = s_pTexture[dwType][dwHitCount];
	}
}


void CBrick::Zap()
{
	if ( IsDestructible() ) {
		dwHitCount = s_Type[dwType].dur - 1;
		pTexture = s_pTexture[dwType][dwHitCount];
	}
}