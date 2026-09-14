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
	CCursor(LPDIRECT3DTEXTURE8 Texture);

	virtual ~CCursor();

	//! Renders the cursor on screen
	/*!
		Renders by rounding the position to the nearest pixel.
		This improves the visual effect at low resolutions.
		\param pSprite	address of the \e ID3DXSprite used for rendering
	*/
	void Render(LPD3DXSPRITE pSprite) const;

	//! Moves the cursor
	/*!
		The function ensures the cursor does not leave the screen.
		\param vMovement	Displacement vector
	*/
	HRESULT Move(const D3DXVECTOR2 & vMovement);
};

#endif
