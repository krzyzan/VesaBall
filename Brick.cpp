// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "EffectSprite.h"

CBrick::SType CBrick::s_Type[BRICK_TYPE_MAX] = { 
	{  1,	1}, 
	{  2,	1}, 
	{  3,	1},
	{  5,	1},
	{  8,	1},
	{ 13,	1}, 
	{ 21,	1}, 
	{ 34,	1},
	{ 55,	1},
	{ 89,	1},
	{144,	BRICK_PERSISTENT}, 
	{233,	3},
};

LPDIRECT3DTEXTURE8 CBrick::s_pTexture[BRICK_TYPE_MAX][DUR_MAX];

CBrick::CBrick( DWORD Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
: CSprite( s_pTexture[Type][0], Size, 0, Position, 0xFFFFFFFF )
{
	dwType = Type;
	dwHitCount = 0;
}


CEffectSprite* CBrick::CreateBlendEffect() const
{
	return new CEffectSprite( pTexture, vSize, vPosition, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0.25f, 0xFFFFFFFF );
}


void CBrick::Hit()
{
	if ( IsDestructible() ) {
		dwHitCount++;
		if ( !IsDestroyed() )
			pTexture = s_pTexture[GetType()][dwHitCount];		//TODO: ugly
	}
}


void CBrick::Zap()
{
	if ( IsDestructible() ) {
		dwHitCount = s_Type[dwType].dur - 1;
		pTexture = s_pTexture[GetType()][dwHitCount];		//TODO: ugly
	}
}