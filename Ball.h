#pragma once

#include "SpriteMoving.h"


const D3DXVECTOR2 BALL_SIZE_MIN = D3DXVECTOR2( 1.0f / 128, 1.0f / 128);
const D3DXVECTOR2 BALL_SIZE_MAX = D3DXVECTOR2( 1.0f / 56, 1.0f / 56 );
const D3DXVECTOR2 BALL_SIZE_AVG = (BALL_SIZE_MIN + BALL_SIZE_MAX) / 2;

const float BALL_SPEED_VAL_MIN = 0.20f;
const float BALL_SPEED_VAL_MAX = 0.80f;
const float BALL_SPEED_VAL_AVG = 0.40f;

    //! Kulka
    /*!
		\warning Przed utworzeniem obiektów klasy konieczne jest wczytanie tekstury	pod adres #spTexture.
    */ 
class CBall :
	public CSpriteMoving  
{
public:
		//! Konstruktor
		/*!
			Tworzy kulkê o normalnym rozmiarze u¿ywaj¹c tekstury #spTexture
			\param Position		Pozycja pocz¹tkowa
			\param Speed		Prêdkoœæ pocz¹tkowa
		*/ 
	CBall( const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );

	virtual ~CBall();

		//! Wykonuje ruch kulki
		/*!
			\copydoc CSpriteMoving::FrameMove(float fElapsedTime)
			Jeœli kulka jest "z³apana" przez deskê, nie robi nic.
			\param fElapsedTime	Czas od ostatniego wywo³ania
		*/ 
	void FrameMove( float fElapsedTime );
		
		//! Ustawia prêdkoœæ do najbli¿szej wartoœci w przedziale <BALL_SPEED_MIN, BALL_SPEED_MAX>
		/*!
			\param vNewSpeed	Nowa prêdkoœæ.
		*/ 
	void SetSpeed( const D3DXVECTOR2 & vNewSpeed );
		
		//! Zmienia kierunek wektora prêdkoœci przy odbiciu od prostok¹tnego obiektu
		/*!
			\param pSprite		WskaŸnik do obiektu
			\param vSide		Wektor punktu wzgledem œrodka kulki w którym nastepuje kolizja
		*/ 
	void Reflect( CSprite* pSprite, const D3DXVECTOR2 & vSide );

		//! Je¿eli \b true, kulka sie nie porusza, domyœlnie \b false
	bool bCatched;

		//! Adres wspólnej tekstury. 
		/*!
			\warning Ustawiæ przed tworzeniem obiektów klasy.
		*/
	static LPDIRECT3DTEXTURE8 spTexture;
};
