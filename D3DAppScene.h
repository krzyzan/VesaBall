#pragma once

#include <vector>
using namespace std;

#include <d3dx8.h>
#include <dinput.h>

	//! Scena
	/*!
		Klasa abstrakcyjna, opisuje pojedyncz¹ scenê - czêœæ programu stanowi¹c¹ logiczn¹ ca³oœæ, 
		jak np. g³ówne menu gry, ekran gry, ekran opcji. Zapewnia proste zarz¹dzanie teksturami.
	*/
class CD3DAppScene
{
public:
		//! Konstruktor
		/*!
			\param d3dDevice	Adres zainicjalizowanego obiekt Direct3D
		*/
	CD3DAppScene( LPDIRECT3DDEVICE8 d3dDevice );
		//! Destruktor
		/*!
			Automatycznie deallokuje wszystkie tekstury za³adowane przy pomocy funkcji #LoadTexture()
		*/
	virtual ~CD3DAppScene();

		//! Tworzy obiekty niezale¿ne od urz¹dzenia graficznego
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT InitDeviceObjects()			= 0;
		//! Tworzy obiekty zale¿ne od urz¹dzenia graficznego
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
			\param didod	Pojedynczy pakiet informacji z bufora
		*/
	virtual HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod )	= 0;
		//! Przetwarza dane wejœciowe z klawiatury
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
			\param didod	Pojedynczy pakiet informacji z bufora
		*/
	virtual HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )= 0;
		//! Wykunuje ruch obiektów sceny
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT FrameMove( float fElapsedTime )				= 0;
		//! Renderuje obiekty sceny
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
		*/
	virtual HRESULT FrameRender()				= 0;
		//! Zwraca wskaŸnik do nastepnej sceny
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
			\return	Jesli scena jeszcze siê nie zakoñczy³a zwraca wskaŸnik \e this
			\return	Jeœli bie¿¹ca scena zakoñczy³a siê, tworzy obiekt nastepnej sceny i zwraca adres.
			\return	Jeœli program ma sie zakoñczyæ zwraca NULL.
		*/
	virtual CD3DAppScene* GetNextScene()		= 0;

protected:
		//! Wczytuje teksturê z pliku
		/*!
			Musi byæ zaimplementowana w klasie dziedziczonej.
			\param strFileName	Nazwa pliku tekstury. Obs³ugiwane formaty: .bmp, .dds, .dib, .jpg, .png, and .tga
			\param pTex			Adres wskaŸnika do tekstury który otrzymuje za³adowan¹ teksturê.
		*/
	HRESULT LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex );
		//! Obiekt Direct3D
	LPDIRECT3DDEVICE8 pD3DDevice;
private:
	vector<LPDIRECT3DTEXTURE8> vtrTexture;
};
