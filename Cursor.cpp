#include "StdAfx.h"
#include "Cursor.h"

const float MOUSE_SPEED = 0.002f;

CCursor::CCursor(SDL_Texture* Texture)
	: CSprite(Texture, Vec2(1.0f / 16, 1.0f / 16), 0,
			  Vec2(1.0f / 2, 0.75f / 2), 0xFFFFFFFF)
{
}

CCursor::~CCursor()
{
}

HRESULT CCursor::Move(const Vec2 & vMovement)
{
	vPosition += vMovement * MOUSE_SPEED;
	Vec2 vTopLeft(0.00f, 0.00f);
	Vec2 vBottomRight(1.00f, 0.75f);
	Vec2Maximize(&vPosition, &vPosition, &vTopLeft);
	Vec2Minimize(&vPosition, &vPosition, &vBottomRight);

	return S_OK;
}

void CCursor::Render(SDL_Renderer* pRenderer) const
{
	// Round the position to the nearest pixel before drawing (improves the
	// visual effect at low resolutions); every other sprite is drawn with
	// full sub-pixel precision.
	Vec2 flooredPos(floor(vPosition.x * RES_X) / RES_X, floor(vPosition.y * RES_X) / RES_X);
	DrawTexture(pRenderer, pTexture, NULL, flooredPos, vSize, fRotation, dwColor, bFlipX, bFlipY);
}
