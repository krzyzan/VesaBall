#include "StdAfx.h"
#include "Brick.h"
#include "Ball.h"
#include "SpriteEffect.h"

CBrick::SType CBrick::s_Type[BRICK_TYPE_MAX] = {
	{0, 0, 0},
	{1, 0, 1},
	{2, 2, 0},
	{3, 4, 1},
	{4, 5, 1},
	{5, 0, 1},
	{6, 0, 1},
	{7, 6, 1},
	{8, 0, 1},
	{9, 0, 1},
	{10, 0, 1},
	{11, 0, 1},
	{12, 0, 1},
	{13, 0, 1},
	{14, 0, 1},
	{15, 0, 1},
	{16, 0, 1},
	{17, 0, 1},
	{18, 0, 1},
	{19, 0, 1},
	{20, 0, 1},
	{21, 2, 1},
	{22, 0, 1},
};

SDL_Texture* CBrick::spTexture[BRICK_TYPE_MAX];

CBrick::CBrick(BYTE Type, const Vec2 & Position, const Vec2 & Size)
	: CSprite(spTexture[Type], Size, 0, Position, 0xFFFFFFFF)
{
	idType = Type;
}

CBrick::~CBrick()
{
}

CSpriteEffect* CBrick::CreateBlendEffect(const Vec2 & vSpeed) const
{
	return new CSpriteEffect(pTexture, vSize, vPosition, vSpeed, Vec2(0, 0), 0xFFFFFFFF, 0.25f);
}
