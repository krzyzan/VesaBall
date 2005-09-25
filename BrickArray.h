#ifndef BRICK_ARRAY_H
#define BRICK_ARRAY_H

#include "Brick.h"

const DWORD NUM_LEVELS = 50;

    //! Tablica cegie³ek
	/*!
		\par
		Przechowuje obiekty #CBrick. Umo¿liwia znalezienie w sta³ym czasie cegie³ki w danym punkcie ekranu.
		Zlicza cegie³ki które mo¿na "zbiæ".
	*/
class CBrickArray
{
public:
		//! Tworzy pust¹ tablicê cegie³ek
		/*
			/param	Size		Liczba kolumn i wierszy
			/param	Position	Pozycja œrodka tablicy cegie³ek
			/param	ScreenSize	Rozmiar tablicy cegie³ek na ekranie
		*/
	CBrickArray( const POINT & Size, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & ScreenSize );

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
			\param	idType	Typ cegie³ki
			\param	pos		Pozycja w tablicy
			\warning Jeœli na tym miejscu jest cegie³ka nale¿y j¹ najpierw usun¹æ!
		*/ 
	void InsertBrick( BYTE idType, const POINT & pos );

		//! Usuwa cegie³kê
		/*!
			Je¿eli na podanej pozycji nie ma cegie³ki, nie robi nic.
			\param	pos		Pozycja w tablicy
		*/ 
	void RemoveBrick( const POINT & pos );

		//! Zwraca \b true jeœli w tablicy nie ma ju¿ cegie³ek do zbicia
	bool Empty() const
		{ return !dwBricksLeft; }

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

		//! Zwraca \b true jeœli wspó³rzêdne nale¿¹ do tablicy
		/*!
			\param	pos	Wspó³rzêdne
		*/
	bool IsValid( const POINT & pos ) const
		{ return (pos.x>=0) && (pos.x<Max.x) && (pos.y>=0) && (pos.y<Max.y);}

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

		//! Obni¿a cegie³ki
		/*!
			Obni¿a te cegie³ki pod którymi jest wolne miejsce.
			Nie rusza "niezniszczalnych" cegie³ek co zapobiega zablokowaniu siê kulki.
		*/
	void FallBricks();

		//! Cegie³ki przylegajace do wybuchowych staj¹ siê wybuchowe
	void ExpandExploding();

		//! Dodaje do listy pozycje wszystkich wybuchowych cegie³ek
	void PushExplosive( list<POINT>* pList );

private:
	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;

	CBrick***	pBrick;
	POINT		Max;

	DWORD dwBricksLeft;
};

#endif

