#ifndef MENUITEM_H
#define MENUITEM_H

#include "Sprite.h"

class CMenuItem :
	public CSprite
{
public:

		//! Konstruktor
		/*!
			Tworzy pozycje menu
			\param Texture		Tekstura
			\param Size			Rozmiar
			\param Position		Pozycja
			\param Color		Kolor
			\param UID			Unikalne ID
		*/ 
	CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, D3DCOLOR Color, HRESULT UID );
	
	virtual ~CMenuItem();

		//! Renderuje pozycjê menu oraz rzucany cieñ.
	void Render( LPD3DXSPRITE pSprite ) const;

		//! Ustawia czy naciœniêto
	void SetPressed( bool bPressed );
	
		//! Ustawia czy podœwietlono
	void SetHighlighted( bool bHighlighted );
	
		//! Zwraca UID
	DWORD GetUID() const 
		{ return dwUID; }

private:
	CSprite* pShadow;
	DWORD dwUID;
};

#endif
