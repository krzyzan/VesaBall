#pragma once
#include "Brick.h"

const BRICK_ARRAY_X = 20;
const BRICK_ARRAY_Y = 20;

const DWORD NUM_LEVELS = 6;

    //! Tablica cegie³ek
	/*!
		\par
		"Pojemnik" na obiekty #CBrick. Umo¿liwia znalezienie cegie³ki w danym punkcie ekranu w czasie sta³ym.
		Zlicza cegie³ki które mo¿na "zbiæ".
	*/
class CBrickArray
{
public:
		//! Tworzy pust¹ tablicê cegie³ek
		/*
			/param	Position	Pozycja œrodka tablicy cegie³ek
			/param	Size		Rozmiar tablicy cegie³ek na ekranie
		*/
	CBrickArray( const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );

	virtual ~CBrickArray();

		//! Usuwa wszystkie cegie³ki z tablicy
	void Clear();

		//! Renderuje tablice cegie³ek
		/*!
			\param pSprite	adres \e ID3DXSprite u¿ywanego do renderowania
		*/ 
    void Render( LPD3DXSPRITE pSprite ) const;

		//! Wstawia cegie³kê do tablicy
		/*!
			\param	type	Typ cegie³ki
			\param	pos		Pozycja w tablicy
		*/ 
	void InsertBrick( DWORD type, const POINT & pos );

		//! Usuwa cegie³kê
		/*!
			Je¿eli na podanej pozycji nie ma cegie³ki, nie robi nic.
			\param	pos		Pozycja w tablicy
		*/ 
	void RemoveBrick( const POINT & pos );

		//! Zwraca \b true jeœli w tablicy nie ma ju¿ cegie³ek do zbicia
	bool Empty() const
		{ return !dwBrickCounter; }

		//! Wczytuje tablicê cegie³ek z pliku
		/*!
			Wszystkie cegie³ki znajduj¹ce siê aktualnie w tablicy s¹ usuwane.
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
		/*!
			\param	vPos	Punkt na ekranie
		*/
	bool Contains( const D3DXVECTOR2 & vPos ) const;

		//! Zwraca wspó³rzêdne w tablicy odpowiadaj¹ce punktowi na ekranie
		/*!
			\param	vPos	Punkt na ekranie
		*/
	POINT GetArrayCoordsAt( const D3DXVECTOR2 & vPos ) const;

		//! Zwraca punkt na ekranie odpowiadaj¹cy wspó³rzêdnym w tablicy
	D3DXVECTOR2 GetPositionAt( const POINT & pos ) const;

		//! Zwraca wskaŸnik do cegie³ki na okreœlonej pozycji
		/*!
			\param	pos		Wspó³rzêdne w tablicy
			\return	WskaŸnik do cegie³ki
		*/
	CBrick* GetBrick( const POINT & pos ) const 
		{ return pBrick[pos.x][pos.y]; }

		//! Zmniejsza wytrzyma³oœæ wszystkich cegie³ek do 1
	void ZapBricks();

		//! Obni¿a cegie³ki.
		/*!
			Obni¿a te cegie³ki pod którymi jest wolne miejsce.
			Nie rusza "niezniszczalnych" cegie³ek.
		*/
	void FallBricks();

private:
	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;

	DWORD dwBrickCounter;
	CBrick*	pBrick[BRICK_ARRAY_X][BRICK_ARRAY_Y];
};