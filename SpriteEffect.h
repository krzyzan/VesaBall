#pragma once
#include "SpriteMoving.h"

//! Efekt znikaj¹cej tekstury.
/*!
	Wyœwietla p³ynnie znikaj¹c¹ teksturê. Gdy skoñczy #HasExpired() zwraca true.
*/
class CSpriteEffect :
	public CSpriteMoving
{
public:
		//! Konstruktor
		/*!
			\param Texture		Tekstura u¿ywana do renderowania.
			\param Size			Rozmiar
			\param Position		Pozycja pocz¹tkowa
			\param Speed		Prêdkoœæ pocz¹tkowa
			\param Accel		Przyspieszenie
			\param Color		Kolor i kana³ alfa s¹ mno¿one przez t¹ wartoœæ. Wartoœæ \c 0xFFFFFFFF utrzymuje orginalny kolor Ÿród³owy.
			\param Duration		Czas trwania efektu, po tym czasie #HasExpired() zwraca \b true
		*/
	CSpriteEffect( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size,
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, 
		DWORD Color, float Duration );
	
	virtual ~CSpriteEffect();

		//! Ruch efektu
		/*!
			Ruch jak w #CSpriteMoving::FrameMove().
			W zaleznoœci od czasu zmienia wartoœæ kana³u alfa.
		*/
	void FrameMove( float fElapsedTime );
		
		//! Zwraca \b true jeœli min¹³ czas trwania efektu 
	bool HasExpired() const;

protected:
	float fRemaining;
	float fDuration;
};
