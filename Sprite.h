#ifndef SPRITE_H
#define SPRITE_H

#include <d3dx8.h>

//! Sprite (tekturowany obiekt 2D)
/*!
	Klasa CSprite u³atwia korzystanie z interfejsu \b ID3DXSprite.
	Uniezale¿nia od rozdzielczoœci u¿ywaj¹c wspó³rzêdnych ekranu 
	tzn. lewy górny \c (0.00f, 0.00f), prawy dolny \c (1.00f, 0.75f)
	Pozycja oraz punkt obrotu ustawiona jest na œrodku sprite'a.
*/

class CSprite  
{
public:
		// Konstruktor
		/*!
			\param Texture	tekstura uzywana do renderowania
			\param Size		rozmiar sprite'a we wspó³rzêdnych ekranu
			\param Rotation	k¹t obrotu
			\param Position	pozycja œrodka we wspó³rzêdnych ekranu
			\param Color	Kolor i kana³ alfa s¹ mno¿one przez t¹ wartoœæ. Wartoœæ \c 0xFFFFFFFF utrzymuje orginalny kolor Ÿród³owy.
		*/
	CSprite( const LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
			float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Color );
	virtual ~CSprite();

		//! Renderuje sprite
		/*!
			\param pSprite Obiekt \b ID3DXSprite u¿ywany do renderowania.
		*/
	virtual void Render( LPD3DXSPRITE pSprite ) const;

		//! Ustawia rozmiar sprite'a przy pomocy skalowania.
	void SetSize( const D3DXVECTOR2 & Size );

		//! Obraca sprite w pionie
	void FlipV() 
		{ vScaling.x *= -1; }

		//! Obraca sprite w poziomie
	void FlipH() 
		{ vScaling.y *= -1; }

		//! Zmienia kolor
		/*!
			\param Color	Kolor i kana³ alfa s¹ mno¿one przez t¹ wartoœæ. Wartoœæ \c 0xFFFFFFFF utrzymuje orginalny kolor Ÿród³owy.
		*/
	void SetColor( D3DCOLOR Color ) 
		{ dwColor = Color; }

	D3DXVECTOR2			vPosition;
	D3DXVECTOR2			vSize;

protected:
	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;
	float				fRotation;
	D3DXVECTOR2			vRotationCenter;
	D3DCOLOR			dwColor;
};

#endif
