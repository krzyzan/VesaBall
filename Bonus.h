#pragma once
#include "SpriteMoving.h"

    //! Bonus
    /*!
		Przed u¿yciem konieczne jest za³adowanie tekstur do tablicy #spTexture.
    */
class CBonus :
	public CSpriteMoving
{
public:
	//! Typy bonusów
	enum TypeEnum
	{
		ThruBrick,		//!< Kulka przenika przez cegie³ki zamiast odbijaæ siê od nich.
		SetOffExploding,//!< Eksploduje wybuchowe cegie³ki
		FireBall,		//!< Kulki przy uderzeniu o cegie³ki wybuchaj¹
		ShootingPaddle,	//!< Deska mo¿e strzelaæ
		GrabPaddle,		//!< Deska ³apie kulki

		ExtraLife,		//!< Dodaje jedno ¿ycie
		LevelWarp,		//!< Przenosi do nastepnego poziomu
		ZapBricks,		//!< Uszkadza cegie³ki, które wymagaj¹ kilku trafieñ
		SlowBall,		//!< Spowalnia kulki
		ExpandExploding,//!< Cegie³ki przylegaj¹ce do cegie³ek wybuchowych staj¹ siê tak¿e wybuchowe
	
		KillPaddle,		//!< Strata ¿ycia
		ShrinkBall,		//!< Zmniejsza kulki
		FastBall,		//!< Przyspiesza kulki
		SuperShrink,	//!< Zwê¿a deskê do minimum
		FallingBricks,	//!< Ka¿de odbicie kulki od deski powoduje ¿e cegie³ki obni¿aj¹ ciê

		ExpandPaddle,	//!< Poszerza deskê
		ShrinkPaddle,	//!< Zwê¿a deskê do minimum
		SplitBall,		//!< Dla ka¿dej kulki tworzy kulkê poruszaj¹c¹ siê prostopadle do niej
		MegaBall,		//!< Powieksza kulki
		EightBall,		//!< Ka¿da kulka zamienia sie w osiem kulek.

		MAX_TYPE		//!< Zarezerwowane, s³u¿y do okreœlenia iloœci typów bonusów
	};

public:
		//! Konstruktor
		/*!
			Tworzy spadaj¹cy bonus. Teksturê pobiera z tablicy #spTextures w zale¿noœci od typu.
			\param Type		Typ bonusa
			\param Position	Pozycja pocz¹tkowa
			\param Speed	Prêdkoœæ pocz¹tkowa
		*/ 
	CBonus( TypeEnum Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );

	virtual ~CBonus();
		
		//! Zwraca typ bonusa
	TypeEnum GetType() const 
		{ return eType; }

		//! Tablica tekstur
		/*!
			Statyczna tablica adresów tekstur, dla poszczególnych typów bonusów. \warning Za³adowaæ tekstury przed tworzeniem obiektów klasy.
		*/
	static LPDIRECT3DTEXTURE8 spTextures[MAX_TYPE];

private:
	TypeEnum eType;
};
