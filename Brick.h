#pragma once
#include "Sprite.h"

class CBall;
class CSpriteEffect;


const DUR_MAX = 4;
const BYTE BRICK_TYPE_MAX = 13;

    //! Cegie³ka
    /*!
		Przed u¿yciem konieczne jest za³adowanie tekstur do tablicy #spTexture.
    */
 class CBrick :
	public CSprite  
{
	struct SType
	{
		DWORD	dwScore;
		bool	bExplosive;
		DWORD	dwDurability;
	};

public:
		//! Konstruktor
		/*!
			\param Type		Typ cegie³ki
			\param Position	Pozycja pocz¹tkowa
			\param Size		Rozmiar
		*/ 
	CBrick( DWORD Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );

	virtual ~CBrick();

		//! Tworzy (alokuje) efekt znikajacej cegie³ki
		/*!
			\return Zaalokowany obiekt #CSpriteEffect
		*/ 
	CSpriteEffect* CreateBlendEffect() const;

		//! Zwiêksza licznik trafieñ
	void Hit();

		//! Jeœli cegie³ka wytrzymuje kilka uderzeñ pi³ki, uszkadza j¹
	void Zap();

		//! Zwraca ile razy trafiona
	DWORD GetHitCount() const 
		{return dwHitCount;}

		//! Zwraca liczbê okreœlaj¹c¹ typ cegie³ki
	DWORD GetType() const 
		{return dwType;}

		//! Zwraca punktacjê za zbicie cegie³ki
	DWORD GetScore() const 
		{return s_Type[dwType].dwScore;}

		//! Czy trafiona ju¿ tyle razy ¿e zniszczona?
	bool IsDestroyed() const 
		{return dwHitCount == s_Type[dwType].dwDurability;}

		//! Czy mo¿e byæ zniszczona?
	bool IsDestructible() const 
		{return s_Type[dwType].dwDurability != DUR_MAX;}

		//! Czy mo¿e byæ zniszczona?
	bool IsExplosive() const 
		{return s_Type[dwType].bExplosive;}

		//!
	static LPDIRECT3DTEXTURE8	spTexture[BRICK_TYPE_MAX][DUR_MAX];

private:
	DWORD dwType;
	DWORD dwHitCount;

	static SType				s_Type[BRICK_TYPE_MAX];
};
