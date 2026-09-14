#include "StdAfx.h"
#include "Sprite.h"

CSprite::CSprite(SDL_Texture* Texture, const Vec2 & Size,
				 float Rotation, const Vec2 & Position, Color Color)
{
	pTexture = Texture;
	vSize = Size;
	fRotation = Rotation;
	vPosition = Position;
	dwColor = Color;
	bFlipX = false;
	bFlipY = false;
}

CSprite::~CSprite()
{
}

void CSprite::SetSize(const Vec2 & Size)
{
	vSize = Size;
}

void CSprite::DrawTexture(SDL_Renderer* pRenderer, SDL_Texture* pTex, const SDL_Rect* pSrcRect,
						  const Vec2 & center, const Vec2 & size, float rotationRadians,
						  Color color, bool flipX, bool flipY)
{
	// Uses the float-precision SDL_RenderCopyExF (SDL_FRect, since SDL 2.0.10)
	// rather than the integer SDL_RenderCopyEx, so sprite motion stays as
	// smooth/sub-pixel-precise as the original D3D8 rendering was.
	SDL_FRect dst;
	dst.w = size.x * RES_X;
	dst.h = size.y * RES_X;
	dst.x = center.x * RES_X - dst.w / 2.0f;
	dst.y = center.y * RES_X - dst.h / 2.0f;

	SDL_FPoint rotCenter = {dst.w / 2.0f, dst.h / 2.0f};

	SDL_SetTextureColorMod(pTex, ColorR(color), ColorG(color), ColorB(color));
	SDL_SetTextureAlphaMod(pTex, ColorA(color));

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (flipX)
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
	if (flipY)
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

	double angleDeg = rotationRadians * 180.0 / PI;

	SDL_RenderCopyExF(pRenderer, pTex, pSrcRect, &dst, angleDeg, &rotCenter, flip);
}

void CSprite::Render(SDL_Renderer* pRenderer) const
{
	DrawTexture(pRenderer, pTexture, NULL, vPosition, vSize, fRotation, dwColor, bFlipX, bFlipY);
}
