// D3DApp.h: interface for the CD3DApp class.
// v1.11
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v1.00
	- Wersja poczatkowa (zrobiona u Laski)

v1.10
	- Obs³uga Alt-Tab
	- Obs³uga mychy (Direct Input)
	- Czyszczenie ekranu na starcie
	- Wiêcej sprawdzania zwracanych kodów b³êdu (SUCCEEDED, FAILED)
	- Zamiana globalnych zmiennych i funkcji na statyczne (¿eby nikt nie rusza³ s_pCurD3DApp)
	- Usuwamy akceleratory bo s¹ do dupy (ESC dalej dzia³a) => resource.h niepotrzebne
	- Funkcja Render nie musi juz wykonywaæ Present (zajmuje sie tym CD3DApp)

v1.11
	- Poprawiona obs³uga myszki (by³ z³y format danych)
	- Usuniêty memory leak zwi¹zany z obiektem D3D
	- Nowy CTimer: dzia³a na starszych komputerach które nie maj¹ High Perforance Timer
*/

#pragma once
#include <d3dx8.h>
#include <dinput.h>
#include "timer.h"

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

class CD3DApp  
{
public:
	CD3DApp( INT ResX, INT ResY );

	// Functions to create, run, pause, and clean up the application
	HRESULT Create( HINSTANCE hInstance );
	HRESULT	Run();

protected:
	virtual HRESULT InitDeviceObjects()			= 0;
	virtual HRESULT RestoreDeviceObjects()		= 0;
	virtual HRESULT FrameMove()					= 0;
	virtual HRESULT Render()					= 0;
	virtual HRESULT InvalidateDeviceObjects()	= 0;
	virtual HRESULT DeleteDeviceObjects()		= 0;


	BOOL	bActive;
	BOOL	bReady;

	// DirectD3D
	LPDIRECT3D8				pD3D;
	LPDIRECT3DDEVICE8		pd3dDevice;

	// DirectInput
	LPDIRECTINPUT8			pDI;
	LPDIRECTINPUTDEVICE8	pDIDevice;

	// Timer
	CTimer	Timer;

private:
	HRESULT InitD3D();
	HRESULT InitDI();
	HRESULT Render3DEnvironment();
	HRESULT Resize3DEnvironment();
	HRESULT	Cleanup3DEnvironment();

protected:
	INT xRes;
	INT yRes;

	LRESULT		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND		hWnd;

	D3DPRESENT_PARAMETERS	d3dpp;

public:
	// Needed for the static WndProc()
	static CD3DApp* s_pCurD3DApp;
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};
