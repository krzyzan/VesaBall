#pragma once

#include "MovingSprite.h"

class CBrick;


const D3DXVECTOR2 BALL_SIZE_MIN = D3DXVECTOR2( 1.0f / 128, 1.0f / 128);
const D3DXVECTOR2 BALL_SIZE_MAX = D3DXVECTOR2( 1.0f / 56, 1.0f / 56 );
const D3DXVECTOR2 BALL_SIZE_AVG = (BALL_SIZE_MIN + BALL_SIZE_MAX) / 2;

const float BALL_SPEED_VAL_MIN = 0.20f;
const float BALL_SPEED_VAL_MAX = 0.80f;
const float BALL_SPEED_VAL_AVG = 0.40f;

    //! Kulka
    /*!
      Przed utworzeniem obiektów klasy konieczne jest wczytanie tekstury
	  pod adres #s_pTexture.
    */ 
class CBall :
	public CMovingSprite  
{
public:
		//! Konstruktor
		/*!
			\param Position		Pozycja pocz¹tkowa
			\param Speed		Prêdkoœæ pocz¹tkowa
		*/ 
	CBall( const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
		//! Ruch kulki
		/*!
			\param fElapsedTime	Czas od ostatniego wywo³ania
		*/ 
	void FrameMove( float fElapsedTime );
		//! Ustawia prêdkoœæ, "zaokraglaj¹c" jej wartoœæ do przedzia³u <BALL_SPEED_MIN, BALL_SPEED_MAX>
		/*!
			\param vNewSpeed	Nowa prêdkoœæ.
		*/ 
	void SetSpeed( const D3DXVECTOR2 & vNewSpeed );
		//! Odbija kulkê od cegie³ki
		/*!
			\param pBrick		WskaŸnik do cegie³ki
			\param vSide		Wektor punktu w którym nastepuje kolizja wzgledem œrodka kulki
		*/ 
	void Reflect( CBrick* pBrick, const D3DXVECTOR2 & vSide );
		//! Je¿eli TRUE, ruchem kulki zajmuje siê #CPaddle, domyslnie FALSE
	bool bCatched;
		//! Adres wspólnej tekstury. \b UWAGA! Ustawiæ przed tworzeniem obiektów klasy.
	static LPDIRECT3DTEXTURE8 s_pTexture;
};
