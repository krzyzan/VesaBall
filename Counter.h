#pragma once
#include "Sprite.h"

	//! Prosty licznik wyœwietlajacy liczby dodatnie
    /*!
		Przed utworzeniem obiektów klasy konieczne jest wczytanie tekstury
		pod adres #spTexture. W teksturze powinny byæ pionowo umieszczone cyfry
		0123456789.
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
			\param DigitPixels	Rozmiar cyfry w teksturze podana w pikselach
			\param NumDigits	Liczba wyœwietlanych cyfr
		*/ 
	CCounter( LONG Value, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position, 
		POINT DigitPixels, DWORD NumDigits );
		
	virtual ~CCounter();

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

		//! Adres wspólnej tekstury.
		/*!
			\warning Ustawiæ przed tworzeniem obiektów klasy.
		*/
	static LPDIRECT3DTEXTURE8 spTexture;
	LONG lValue;

private:
	POINT dwDigitPixels;
	DWORD dwNumDigits;
	float fValue;
};
