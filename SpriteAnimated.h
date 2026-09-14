#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include "SpriteEffect.h"

//! Animowany sprite
/*!
	Zajmuje siê wyœwietlaniem animacji. Gdy skoñczy #HasExpired() zwraca true.
*/

class CSpriteAnimated :
	public CSpriteEffect
{
public:
		//! Konstruktor
		/*!
			\param Texture		Tekstura zawieraj¹ce 8x8 ramek u¿ywanych do renderowania.
			\param Size			Rozmiar
			\param Rotation		Obrót w radianach
			\param Position		Pozycja pocz¹tkowa
			\param Speed		Prêdkoœæ pocz¹tkowa
			\param Accel		Przyspieszenie
			\param Color		Kolor i kana³ alfa s¹ mno¿one przez t¹ wartoœæ. Wartoœæ \c 0xFFFFFFFF utrzymuje orginalny kolor Ÿród³owy.
			\param Duration		Czas trwania, po tym czasie #HasExpired() zwraca \b true
			\param FirstFrame	Pierwsza ramka animacji
			\param LastFrame	Ostatnia ramka animacji
            \param FramePixels	Rozmiar ramki w teksturze podany pikselach
		*/
	CSpriteAnimated( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size,  float Rotation,
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, DWORD Color, 
		float Duration, DWORD FirstFrame, DWORD LastFrame, const POINT & FramePixels );
	
	virtual ~CSpriteAnimated();

		//! Ruch animacji
		/*!
			Oblicza bie¿¹ca ramkê animacji.
			\param fElapsedTime	Czas od ostatniego wywo³ania
		*/ 
	void FrameMove( float fElapsedTime );

		//! Renderuje animacjê
		/*!
			Jesli z³apano kulki renderuje te¿ efekt b³yskawicy.
			\param pSprite Obiekt \b ID3DXSprite u¿ywany do renderowania.
		*/
	void Render( LPD3DXSPRITE pSprite ) const;

private:
	DWORD dwFirstFrame;
	DWORD dwLastFrame;
	POINT ptFramePixels;
};

#endif
