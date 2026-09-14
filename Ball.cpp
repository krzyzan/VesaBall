// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ball.h"
#include "Paddle.h"

SDL_Texture* CBall::spTexture;

CBall::CBall(const Vec2 & Position, const Vec2 & Speed)
	: CSpriteMoving(spTexture, BALL_SIZE_AVG, 0,
					Position, Speed, Vec2(0, 0), 0xFFFFFFFF)
{
	bCatched = false;
}

CBall::~CBall()
{
}

void CBall::FrameMove(float fElapsedTime)
{
	if (bCatched)
		return;

	CSpriteMoving::FrameMove(fElapsedTime);
}

void CBall::SetSpeed(const Vec2 & vNewSpeed)
{
	float fNewSpeedVal = Vec2Length(&vNewSpeed);
	fNewSpeedVal = min(fNewSpeedVal, BALL_SPEED_VAL_MAX);
	fNewSpeedVal = max(fNewSpeedVal, BALL_SPEED_VAL_MIN);
	Vec2Normalize(&vSpeed, &vNewSpeed);
	vSpeed *= fNewSpeedVal;
}

void CBall::Reflect(const CSprite* pSprite, const Vec2 & vSide)
{
	if (vSide.y > 0)
	{
		vPosition.y = 2 * pSprite->vPosition.y - vSize.y - vPosition.y - pSprite->vSize.y;
		vSpeed.y *= -1;
	}

	if (vSide.y < 0)
	{
		vPosition.y = 2 * pSprite->vPosition.y + vSize.y - vPosition.y + pSprite->vSize.y;
		vSpeed.y *= -1;
	}

	if (vSide.x > 0)
	{
		vPosition.x = 2 * pSprite->vPosition.x - vSize.x - vPosition.x - pSprite->vSize.x;
		vSpeed.x *= -1;
	}

	if (vSide.x < 0)
	{
		vPosition.x = 2 * pSprite->vPosition.x + vSize.x - vPosition.x + pSprite->vSize.x;
		vSpeed.x *= -1;
	}
};
