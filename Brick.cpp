// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "Bonus.h"
#include "EffectSprite.h"

CBrick::CBrick( BYTE Type, LPDIRECT3DTEXTURE8* Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
	: CSprite( Texture[Type], Size, 0, Position, brickDescTable[Type].dwBlending )
{
	dwType = Type;
	dwDurability = brickDescTable[Type].dwDurability;
}


CBrick::~CBrick()
{
}


CEffectSprite* CBrick::CreateEffect()
{
	return new CEffectSprite( pTexture, vSize, vPosition, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0.25f, 0xFFFFFFFF );
}


void CBrick::ReflectBall( CBall* pBall, const D3DXVECTOR2 & vSide )
{
	if (vSide.y){
		pBall->vSpeed.y *= -1;
		pBall->vPosition.y = 2*(vPosition.y - vSide.y) - pBall->vPosition.y;
		pBall->vPosition.y -= (vSide.y > 0) ? vSize.y : (-vSize.y);
	}

	if (vSide.x) {
		pBall->vSpeed.x *= -1;
		pBall->vPosition.x = 2*(vPosition.x - vSide.x) - pBall->vPosition.x;
		pBall->vPosition.x -= (vSide.x > 0) ? vSize.x : (-vSize.x);
	}
};