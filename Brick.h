#pragma once
#include "Sprite.h"

class CBall;
class CEffectSprite;


const DUR_MAX = 4;
const BYTE BRICK_TYPE_MAX = 13;

    //! Cegie³ka
    /*!
		Przed u¿yciem konieczne jest za³adowanie tekstur do tablicy #s_pTexture.
    */
 class CBrick :
	public CSprite  
{
	struct SType
	{
		DWORD	dwScore;
		bool	bExplosive;
		DWORD	dur;
	};

public:
		//! Konstruktor
		/*!
			\param Type		Typ cegie³ki
			\param Position	Pozycja pocz¹tkowa
			\param Size		Rozmiar
		*/ 
	CBrick( DWORD Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
		//! Zwiêksza licznik trafieñ
	void Hit();
		//! Ustawia licznik trafieñ tak, ¿e wystarczy juz tylko jedno trafienie aby "zbiæ" cegie³kê
	void Zap();
		//! Tworzy (alokuje) efekt znikajacej cegie³ki
		/*!
			\return Zaalokowany obiekt #CEffectSprite
		*/ 
	CEffectSprite* CreateBlendEffect() const;
		//! Zwraca ile razy trafiona
	DWORD GetHitCount() const {return dwHitCount;}
		//! Zwraca liczbê okreœlaj¹c¹ typ cegie³ki
	DWORD GetType() const {return dwType;}
		//! Zwraca punktacjê za zbicie cegie³ki
	DWORD GetScore() const {return s_Type[dwType].dwScore;}
		//! Czy trafiona ju¿ tyle razy ¿e zniszczona?
	bool IsDestroyed() const {return dwHitCount == s_Type[dwType].dur;}
		//! Czy mo¿e byæ zniszczona?
	bool IsDestructible() const {return s_Type[dwType].dur != DUR_MAX;}
		//! Czy mo¿e byæ zniszczona?
	bool IsExplosive() const {return s_Type[dwType].bExplosive;}

	static SType				s_Type[BRICK_TYPE_MAX];
	static LPDIRECT3DTEXTURE8	s_pTexture[BRICK_TYPE_MAX][DUR_MAX];

private:
	DWORD dwType;
	DWORD dwHitCount;
};
