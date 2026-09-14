#include "StdAfx.h"
#include "Bonus.h"

LPDIRECT3DTEXTURE8 CBonus::spTextures[CBonus::MAX_TYPE];

CBonus::CBonus( TypeEnum Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
	: CSpriteMoving( spTextures[Type], D3DXVECTOR2(1.0f/32, 1.0f/32), 0, Position, Speed, D3DXVECTOR2(0, GRAV_ACCEL), 0xFFFFFFFF )
{
	eType = Type;
}

CBonus::~CBonus()
{
}
