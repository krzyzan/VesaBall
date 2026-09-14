#include "StdAfx.h"
#include "SpriteAnimated.h"

CSpriteAnimated::CSpriteAnimated(SDL_Texture* Texture, const Vec2 & Size, float Rotation,
								 const Vec2 & Position, const Vec2 & Speed, const Vec2 & Accel, Color Color,
								 float Duration, DWORD FirstFrame, DWORD LastFrame, const POINT & FramePixels)
	: CSpriteEffect(Texture, Size, Position, Speed, Accel, Color, Duration)
{
	fRotation = Rotation;

	dwFirstFrame = FirstFrame;
	dwLastFrame = LastFrame;
	ptFramePixels = FramePixels;
}

CSpriteAnimated::~CSpriteAnimated()
{
}

void CSpriteAnimated::Render(SDL_Renderer* pRenderer) const
{
	LONG lFrame = LONG((1.0f - fRemaining / fDuration) * (dwLastFrame - dwFirstFrame)) + dwFirstFrame;
	SDL_Rect SrcRect;
	SrcRect.x = (lFrame / 8) * ptFramePixels.x;
	SrcRect.y = (lFrame % 8) * ptFramePixels.y;
	SrcRect.w = ptFramePixels.x;
	SrcRect.h = ptFramePixels.y;

	DrawTexture(pRenderer, pTexture, &SrcRect, vPosition, vSize, fRotation, dwColor, bFlipX, bFlipY);
}

void CSpriteAnimated::FrameMove(float fElapsedTime)
{
	fRemaining -= fElapsedTime;
}
