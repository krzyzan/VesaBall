#ifndef MENUITEM_H
#define MENUITEM_H

#include "Sprite.h"

class CMenuItem :
	public CSprite
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
	CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, D3DCOLOR Color, HRESULT UID );
	
	virtual ~CMenuItem();

		//! Renders the menu item and its cast shadow.
	void Render( LPD3DXSPRITE pSprite ) const;

		//! Sets whether it's pressed
	void SetPressed( bool bPressed );
	
		//! Sets whether it's highlighted
	void SetHighlighted( bool bHighlighted );
	
		//! Returns the UID
	DWORD GetUID() const 
		{ return dwUID; }

private:
	CSprite* pShadow;
	DWORD dwUID;
};

#endif
