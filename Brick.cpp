// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "SpriteEffect.h"

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
	{144,	DUR_MAX}, 
	{233,	3},
	{377,	2},
};

LPDIRECT3DTEXTURE8 CBrick::spTexture[BRICK_TYPE_MAX][DUR_MAX];

CBrick::CBrick( DWORD Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
: CSprite( spTexture[Type][0], Size, 0, Position, 0xFFFFFFFF )
{
	dwType		= Type;
	dwHitCount	= 0;
}


CBrick::~CBrick()
{
}


void CBrick::Hit()
{
	if ( IsDestructible() ) {
		dwHitCount++;
		if ( !IsDestroyed() )
			pTexture = spTexture[dwType][dwHitCount];
	}
}


void CBrick::Zap()
{
	if ( IsDestructible() ) {
		dwHitCount = s_Type[dwType].dwDurability - 1;
		pTexture = spTexture[dwType][dwHitCount];
	}
}

CSpriteEffect* CBrick::CreateBlendEffect( const D3DXVECTOR2 & vSpeed ) const
{
	return new CSpriteEffect( pTexture, vSize, vPosition, vSpeed, D3DXVECTOR2(0, 0), 0xFFFFFFFF, 0.25f );
}
