#pragma once
#include "Brick.h"

const BRICK_ARRAY_X = 20;
const BRICK_ARRAY_Y = 20;

const DWORD NUM_LEVELS = 3;

    //! Tablica cegie³ek
class CBrickArray
{
public:
	CBrickArray();
	virtual ~CBrickArray();

		//! Renderuje wszystkie cegie³ki
		/*!
			\param pSprite	adres \e ID3DXSprite u¿ywanego do renderowania
		*/ 
    void Render( LPD3DXSPRITE pSprite ) const;

		//! "Os³abia" wszystkie cegie³ki
	void ZapBricks();

		//! Spadaj¹ce cegie³ki
	void FallBricks();

		//! Czy dany punkt na ekranie nale¿y do tablicy cegie³ek?
	bool Contains( const D3DXVECTOR2 & vPos ) const;

		//! Zwraca wspó³rzêdne w tablicy odpowiadaj¹ce punktowi na ekranie
	POINT GetArrayCoords( const D3DXVECTOR2 & vPos ) const;

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

		//! Czy w tablicy s¹ cegie³ki?
		/*!
			Niezniszczalne cegie³ki nie s¹ liczone.
		*/
	bool Empty() const
		{ return (dwBrickCounter == 0); }

		//! Zwraca adres cegie³ki w tablicy
		/*!
			\param	pos		Wspó³rzêdne w tablicy
			\return	Adres cegie³ki
		*/ 
	CBrick* GetBrick( const POINT & pos ) const {return pBrick[pos.x][pos.y];}

		//! Kasuje wszystkie cegie³ki
	void Clear();
		//! Wczytuje tablicê cegie³ek z pliku
		/*!
			\b UWAGA: brak obs³ugi b³êdów
			Wszystkie cegie³ki znajduj¹ce siê w tablicy s¹ usuwane.
			\param	strFileName	Nazwa pliku
		*/ 
	void Load( DWORD dwLevelNum );

		//! Zapisuje tablicê cegie³ek do pliku
		/*!
			\b UWAGA: brak obs³ugi b³êdów
			\param	dwLevelNum	Numer poziomu
		*/ 
	void Save( DWORD dwLevelNum ) const;

private:
	CBrick*	pBrick[BRICK_ARRAY_X][BRICK_ARRAY_Y];
	DWORD dwBrickCounter;

	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;
};