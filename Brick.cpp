// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "Bonus.h"
#include "EffectSprite.h"

CBrick::STypeDesc CBrick::s_TypeDesc[CBrick::TYPE_MAX] = { 
	{0,	1			}, 
	{1,	1			}, 
	{2, 1			},
	{3,	0xFFFFFFFF	}, 
	{4,	3			},
};

CBrick::CBrick( STypeDesc* BrickType, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
: CSprite( BrickType->pTexture[0], Size, 0, Position, 0xFFFFFFFF )
{
	pTypeDesc = BrickType;
	dwHitCounter = 0;
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
	
	SetHitCounter( dwHitCounter+1 );
};

void CBrick::SetHitCounter( DWORD HitCounter )
{
	if (pTypeDesc->dur != 0xFFFFFFFF) {
		dwHitCounter = HitCounter;
		if (dwHitCounter < pTypeDesc->dur)		//TODO: ugly
			pTexture = pTypeDesc->pTexture[dwHitCounter];
	}
}