#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include "Vec2.h"
#include "Color.h"

//! Sprite (textured 2D object)
/*!
	The CSprite class simplifies working with SDL's 2D renderer.
	It makes rendering resolution-independent by using screen coordinates,
	i.e. top-left (0.00f, 0.00f), bottom-right (1.00f, 0.75f)
	The position and rotation center are set at the sprite's center.
*/

class CSprite
{
public:
	//! Constructor
	/*!
		\param Texture	texture used for rendering
		\param Size		sprite size in screen coordinates
		\param Rotation	rotation angle, in radians
		\param Position	position of the center in screen coordinates
		\param Color	The color and alpha channel are multiplied by this value. The value 0xFFFFFFFF preserves the original source color.
	*/
	CSprite(SDL_Texture* Texture, const Vec2 & Size,
			float Rotation, const Vec2 & Position, Color Color);
	virtual ~CSprite();

	//! Renders the sprite
	/*!
		\param pRenderer The SDL renderer used for rendering.
	*/
	virtual void Render(SDL_Renderer* pRenderer) const;

	//! Sets the sprite's size.
	void SetSize(const Vec2 & Size);

	//! Flips the sprite horizontally
	void FlipV()
	{
		bFlipX = !bFlipX;
	}

	//! Flips the sprite vertically
	void FlipH()
	{
		bFlipY = !bFlipY;
	}

	//! Changes the color
	/*!
		\param Color	The color and alpha channel are multiplied by this value. The value 0xFFFFFFFF preserves the original source color.
	*/
	void SetColor(Color Color)
	{
		dwColor = Color;
	}

	Vec2 vPosition;
	Vec2 vSize;

protected:
	//! Draws a (possibly sub-rected) region of a texture into the renderer,
	//! scaled to 'size' screen-space units, centered at 'center', rotated
	//! and flipped. Shared by CSprite, CCounter and CSpriteAnimated, all of
	//! which draw a texture region at an arbitrary on-screen size.
	static void DrawTexture(SDL_Renderer* pRenderer, SDL_Texture* pTex, const SDL_Rect* pSrcRect,
							const Vec2 & center, const Vec2 & size, float rotationRadians,
							Color color, bool flipX, bool flipY);

	SDL_Texture* pTexture;
	bool bFlipX;
	bool bFlipY;
	float fRotation;
	Color dwColor;
};

#endif
