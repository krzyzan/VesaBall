#pragma once

#include <vector>
using namespace std;

#include <d3dx8.h>
#include <dinput.h>

	//! Scena
	/*!
		Klasa abstrakcyjna, opisuje pojedyncz¹ scenê - czêœæ programu stanowi¹c¹ logiczn¹ ca³oœæ, 
		jak np. g³ówne menu gry, ekran wyników, menu opcji itp. Zapewnia proste zarz¹dzanie teksturami.
	*/
class CD3DScene
{
public:
		//! Konstruktor
		/*!
			\param d3dDevice	Adres zainicjalizowanego obiektu Direct3D
		*/
	CD3DScene( LPDIRECT3DDEVICE8 d3dDevice );
		
		//! Destruktor
		/*!
			Automatycznie zwalnia wszystkie tekstury za³adowane przy pomocy funkcji #LoadTexture()
		*/
	virtual ~CD3DScene();

		//! Tworzy obiekty sceny niezale¿ne od urz¹dzenia graficznego
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT InitDeviceObjects()			= 0;
		
		//! Tworzy obiekty sceny zale¿ne od urz¹dzenia graficznego
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT RestoreDeviceObjects()		= 0;
		
		//! Usuwa obiekty zale¿ne od urz¹dzenia graficznego
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT InvalidateDeviceObjects()	= 0;
		
		//! Usuwa obiekty niezale¿ne od urz¹dzenia graficznego
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT DeleteDeviceObjects()		= 0;

		//! Przetwarza dane wejœciowe z myszki
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
			\param didod	Dane z bufora
		*/
	virtual HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod )	= 0;
		
		//! Przetwarza dane wejœciowe z klawiatury
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
			\param didod	Dane z bufora
		*/
	virtual HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )= 0;
		
		//! Wykunuje ruch obiektów sceny
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT FrameMove( float fElapsedTime )	= 0;
		
		//! Renderuje obiekty sceny
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT FrameRender()					= 0;
		
		//! Zwraca wskaŸnik do nastepnej sceny
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
			Jeœli wchodzimy do sceny podrzednej (np. z menu g³ównego do menu opcji) 
			funkcja powinna utworzyæ obiekt dziedziczony od #CD3DScene i zwróciæ jej adres.
			Jeœli scena zakoñczy³a siê powinna zwróciæ \b NULL. U¿ywana tylko w #CD3DApp
			\return	Adres bie¿¹cej sceny
			
		*/
	virtual CD3DScene* GetNextScene()		= 0;

protected:
		//! Wczytuje teksturê z pliku. 
		/*!
			Tekstura zostanie automatycznie zwolniona w destruktorze klasy.
			\param strFileName	Nazwa pliku tekstury. Obs³ugiwane formaty: .bmp, .dds, .dib, .jpg, .png, and .tga
			\param pTex			Adres wskaŸnika do tekstury który otrzymuje za³adowan¹ teksturê.
		*/
	HRESULT LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex );

		//! Obiekt Direct3D
	LPDIRECT3DDEVICE8 pD3DDevice;

private:
	vector<LPDIRECT3DTEXTURE8> vctrTextures;
};
