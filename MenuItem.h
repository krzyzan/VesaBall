#ifndef MENUITEM_H
#define MENUITEM_H

#include "Sprite.h"

class CMenuItem : public CSprite
{
public:
	//! Constructor
	/*!
		Creates a menu item
		\param Texture		Texture
		\param Size			Size
		\param Position		Position
		\param Color		Color
		\param UID			Unique ID
	*/
	CMenuItem(SDL_Texture* Texture, const Vec2 & Size,
			  const Vec2 & Position, Color Color, HRESULT UID);

	virtual ~CMenuItem();

	//! Renders the menu item and its cast shadow.
	void Render(SDL_Renderer* pRenderer) const;

	//! Sets whether it's pressed
	void SetPressed(bool bPressed);

	//! Sets whether it's highlighted
	void SetHighlighted(bool bHighlighted);

	//! Returns the UID
	DWORD GetUID() const
	{
		return dwUID;
	}

private:
	CSprite* pShadow;
	DWORD dwUID;
};

#endif
