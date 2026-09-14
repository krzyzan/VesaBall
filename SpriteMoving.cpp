#include "StdAfx.h"
#include "SpriteMoving.h"

CSpriteMoving::CSpriteMoving(SDL_Texture* Texture, const Vec2 & Size, float Rotation,
							 const Vec2 & Position, const Vec2 & Speed, const Vec2 & Accel, Color Color)
	: CSprite(Texture, Size, Rotation, Position, Color)
{
	vOldPosition = vPosition;
	vSpeed = Speed;
	vAccel = Accel;
}

CSpriteMoving::~CSpriteMoving()
{
}

void CSpriteMoving::FrameMove(float fElapsedTime)
{
	vOldPosition = vPosition;
	vSpeed += vAccel * fElapsedTime;
	vPosition += vSpeed * fElapsedTime;

	// bounce off the walls
	if (vPosition.x - vSize.x / 2 < BOARD_L)
	{
		vPosition.x = 2 * BOARD_L - vPosition.x + vSize.x;
		vSpeed.x *= -1;
	}

	if (vPosition.x + vSize.x / 2 > BOARD_R)
	{
		vPosition.x = 2 * BOARD_R - vPosition.x - vSize.x;
		vSpeed.x *= -1;
	}

	if (vPosition.y - vSize.y / 2 < BOARD_T)
	{
		vPosition.y = 2 * BOARD_T - vPosition.y + vSize.y;
		vSpeed.y *= -1;
	}
}

bool CSpriteMoving::HasExpired() const
{
	return vPosition.y - vSize.y / 2 > BOARD_B;
}

Vec2 CSpriteMoving::GetCollisionSide(CSprite* pSprite) const
{
	if (fabs(vOldPosition.x - pSprite->vPosition.x) < vSize.x / 2 + pSprite->vSize.x / 2 &&
		fabs(vPosition.y - pSprite->vPosition.y) < vSize.y / 2 + pSprite->vSize.y / 2)
		return Vec2(0, (vSpeed.y > 0) ? vSize.y / 2 : -vSize.y / 2);

	if (fabs(vOldPosition.y - pSprite->vPosition.y) < vSize.y / 2 + pSprite->vSize.y / 2 &&
		fabs(vPosition.x - pSprite->vPosition.x) < vSize.x / 2 + pSprite->vSize.x / 2)
		return Vec2((vSpeed.x > 0) ? vSize.x / 2 : -vSize.x / 2, 0);

	return Vec2(0, 0);
}
