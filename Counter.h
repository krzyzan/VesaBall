#pragma once
#include "Sprite.h"

	//! Prosty licznik wyœwietlajacy liczby dodatnie
    /*!
		Przed utworzeniem obiektów klasy konieczne jest wczytanie tekstury
		pod adres #s_pTexture. W teksturze powinny byæ pionowo umieszczone cyfry
		01234567890.
    */ 
class CCounter :
	public CSprite
{
public:
		//! Konstruktor
		/*!
			\param Value		Wartoœæ pocz¹tkowa
			\param Size			Rozmiar
			\param Position		Pozycja
			\param DigitHeight	Wysokoœæ pojedynczej cyfry w pikselach
			\param NumDigits	Iloœæ wyœwietlanych cyfr
		*/ 
	CCounter( LONG Value, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position, 
		DWORD DigitHeight, DWORD NumDigits );
		//! Renderuje licznik
		/*!
			\param pSprite	adres \e ID3DXSprite u¿ywanego do renderowania
		*/ 
	void Render( LPD3DXSPRITE pSprite ) const;
		//! Aktualizuje wyœwietlan¹ liczbê 
		/*!
			\param fElapsedTime Czas od ostatniego wywo³ania
		*/
	void Update( float fElapsedTime );
		//! Adres wspólnej tekstury. \b UWAGA! Ustawiæ przed tworzeniem obiektów klasy.
	static LPDIRECT3DTEXTURE8 s_pTexture;

	LONG lValue;

private:
	DWORD dwDigitHeight;
	DWORD dwNumDigits;

	float fValue;
};
