#pragma once
#include "Brick.h"

const BRICK_ARRAY_X = 20;
const BRICK_ARRAY_Y = 20;

const DWORD NUM_LEVELS = 3;

    //! Tablica cegie³ek
class CBrickArray
{
public:
		//! Tworzy pust¹ tablicê cegie³ek
	CBrickArray();

		//! Usuwa wszystkie cegie³ki w tablicy
	virtual ~CBrickArray();

		//! Renderuje wszystkie cegie³ki
		/*!
			\param pSprite	adres \e ID3DXSprite u¿ywanego do renderowania
		*/ 
    void Render( LPD3DXSPRITE pSprite ) const;

		//! Wstawia cegie³kê do tablicy
		/*!
			\param	type	Typ cegie³ki
			\param	pos		Wspó³rzêdne w tablicy
		*/ 
	void InsertBrick( DWORD type, const POINT & pos );

		//! Usuwa cegie³kê
		/*!
			Je¿eli na podanej pozycji nie ma cegie³ki, nie robi nic.
			\param	pos		Wspó³rzêdne w tablicy
		*/ 
	void RemoveBrick( const POINT & pos );

		//! Zwraca liczbê cegie³ek do zniszczenia
	bool Empty() const
		{ return !dwBrickCounter; }

		//! Wczytuje tablicê cegie³ek z pliku
		/*!
			Wszystkie cegie³ki znajduj¹ce siê w tablicy s¹ usuwane.
			\param	dwLevelNum	Numer poziomu
			\warning Brak obs³ugi b³êdów
		*/ 
	void Load( DWORD dwLevelNum );

		//! Zapisuje tablicê cegie³ek do pliku
		/*!
			\param	dwLevelNum	Numer poziomu
			\warning Brak obs³ugi b³êdów
		*/ 
	void Save( DWORD dwLevelNum ) const;

		//! Zwraca czy dany punkt na ekranie nale¿y do tablicy cegie³ek
	bool Contains( const D3DXVECTOR2 & vPos ) const;

		//! Zwraca wspó³rzêdne w tablicy odpowiadaj¹ce punktowi na ekranie
	POINT GetArrayCoords( const D3DXVECTOR2 & vPos ) const;

		//! Zwraca wskaŸnik do cegie³ki na okreœlonej pozycji
		/*!
			\param	pos		Wspó³rzêdne w tablicy
			\return	WskaŸnik do cegie³ki
		*/ 
	CBrick* GetBrick( const POINT & pos ) const 
		{ return pBrick[pos.x][pos.y]; }

		//! "Os³abia" wszystkie cegie³ki
	void ZapBricks();

		//! Obni¿a cegie³ki.
		/*!
			Obni¿a cegie³ki te cegie³ki pod którymi jest wolne miejsce.
			Nie rusza niezniszczalnych cegie³ek.
		*/
	void FallBricks();

private:
	CBrick*	pBrick[BRICK_ARRAY_X][BRICK_ARRAY_Y];
	DWORD dwBrickCounter;

	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;
};