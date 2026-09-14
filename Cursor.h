#ifndef CURSOR_H
#define CURSOR_H

#include "Sprite.h"

//! Mouse cursor
class CCursor : public CSprite
{
public:
	//! Constructor
	/*!
		Creates the cursor at the center of the screen
	*/
	CCursor(SDL_Texture* Texture);

	virtual ~CCursor();

	//! Renders the cursor on screen
	/*!
		Renders by rounding the position to the nearest pixel.
		This improves the visual effect at low resolutions.
		\param pRenderer	the SDL renderer used for rendering
	*/
	void Render(SDL_Renderer* pRenderer) const;

	//! Moves the cursor
	/*!
		The function ensures the cursor does not leave the screen.
		\param vMovement	Displacement vector
	*/
	HRESULT Move(const Vec2 & vMovement);
};

#endif
