#include "StdAfx.h"
#include "Bonus.h"

SDL_Texture* CBonus::spTextures[CBonus::MAX_TYPE];

CBonus::CBonus(TypeEnum Type, const Vec2 & Position, const Vec2 & Speed)
	: CSpriteMoving(spTextures[Type], Vec2(1.0f / 32, 1.0f / 32), 0, Position, Speed, Vec2(0, GRAV_ACCEL), 0xFFFFFFFF)
{
	eType = Type;
}

CBonus::~CBonus()
{
}
