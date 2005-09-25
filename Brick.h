#ifndef BRICK_H
#define BRICK_H

#include "Sprite.h"

class CBall;
class CSpriteEffect;


const BYTE BRICK_TYPE_MAX = 23;
const BYTE BRICK_TYPE_EXPL = 8;

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
		BYTE	idNextType;
		bool	bMustHit;
	};

public:
		//! Konstruktor
		/*!
			\param Type		Typ cegie³ki
			\param Position	Pozycja pocz¹tkowa
			\param Size		Rozmiar
		*/ 
	CBrick( BYTE Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );

	virtual ~CBrick();

		//! Tworzy (alokuje) efekt znikajacej cegie³ki
		/*!
			\return Zaalokowany obiekt #CSpriteEffect
		*/ 
	CSpriteEffect* CreateBlendEffect( const D3DXVECTOR2 & vSpeed ) const;

		//! Zwraca indeks typu cegie³ki
	BYTE GetType() const 
		{return idType;}

		//! Zwraca indeks typu cegie³ki przy uderzeniu
	BYTE GetNextType() const 
		{return s_Type[idType].idNextType;}

		//! Zwraca punktacjê za trafienie cegie³ki
	DWORD GetScore() const 
		{return s_Type[idType].dwScore;}

		//! Zwraca \b true jeœli trzeba zbiæ cegie³kê ¿eby przejœæ poziom
	bool MustHit() const 
		{return s_Type[idType].bMustHit;}

		//! Zwraca \b true jeœli wybuchowa
	bool IsExplosive() const 
		{return idType == BRICK_TYPE_EXPL;}

		//! Tablica tekstur
		/*!
			Statyczna tablica adresów tekstur, dla poszczególnych typów cegie³ek. 
			\warning Za³adowaæ tekstury przed tworzeniem obiektów klasy.
		*/
	static LPDIRECT3DTEXTURE8	spTexture[BRICK_TYPE_MAX];

private:
	BYTE idType;

	static SType				s_Type[BRICK_TYPE_MAX];
};

#endif
