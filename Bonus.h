#pragma once
#include "MovingSprite.h"

    //! Bonus
    /*!
		Przed u¿yciem konieczne jest za³adowanie tekstur do tablicy #s_pTexture.
    */
class CBonus :
	public CMovingSprite
{
public:
	//! Typy bonusów
	enum TypeEnum
	{
		ThruBrick,
		SetOffExploding,
		FireBall,
		ShootingPaddle,
		GrabPaddle,

		ExtraLife,
		LevelWarp,
		ZapBricks,
		SlowBall,
		ExpandExploding,
	
		KillPaddle,
		ShrinkBall,
		FastBall,
		SuperShrink,
		FallingBricks,

		ExpandPaddle,
		ShrinkPaddle,
		SplitBall,
		MegaBall,
		EightBall,

		MAX_TYPE
	};

public:
		//! Konstruktor
		/*!
			\param Type		Typ bonusa
			\param Position	Pozycja pocz¹tkowa
			\param Speed	Prêdkoœæ pocz¹tkowa
		*/ 
	CBonus( TypeEnum Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
		//! Zwraca typ bonusa
	TypeEnum GetType() const 
		{ return eType; }

		//! Statyczna tablica adresów tekstur, dla poszczególnych typów bonusów. \b UWAGA! Ustawiæ przed tworzeniem obiektów klasy.
	static LPDIRECT3DTEXTURE8 s_pTextures[MAX_TYPE];

private:
	TypeEnum eType;
};
