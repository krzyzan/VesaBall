#ifndef CURSOR_H
#define CURSOR_H

#include "Sprite.h"

	//! Kursor myszy
class CCursor :
	public CSprite
{
public:
	
	//! Konstruktor
	/*! 
		Tworzy kursor na œrodku ekranu
	*/
	CCursor( LPDIRECT3DTEXTURE8 Texture );
	
	virtual ~CCursor();

	//! Renderuje kursor na ekranie
	/*! 
		Renderuje zaokr¹glaj¹c pozycjê do piksela.
		Poprawia to efekt wizualny w niskich rozdzielczoœciach.
        \param pSprite	adres \e ID3DXSprite u¿ywanego do renderowania
	*/
	void Render( LPD3DXSPRITE pSprite ) const;
	
	//! Przesuwa kursor
	/*! 
		Funkcja dba o to ¿eby kursor nie opuœci³ ekranu.
		\param vMovement	Wektor przesuniecia
	*/
	HRESULT Move( const D3DXVECTOR2 & vMovement );
};

#endif
