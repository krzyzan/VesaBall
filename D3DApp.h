// D3DApp.h: interface for the CD3DApp class.
// ver 1.1
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v1.0
	Wersja poczatkowa (zrobiona u Laski)
v1.1
	Obs³uga Alt-Tab
	Obs³uga mychy (Direct Input)
	Czyszczenie ekranu na starcie
	Wiêcej sprawdzania zwracanych kodów b³êdu (SUCCEEDED, FAILED)
	Zamiana globalnych zmiennych i funkcji na statyczne (¿eby nikt nie rusza³ s_pCurD3DApp)
	Usuwamy akceleratory bo s¹ do dupy (ESC dalej dzia³a) => resource.h niepotrzebne
	Funkcja Render nie musi juz wykonywaæ Present (zajmuje sie tym CD3DApp)
*/


#pragma once
#define DIRECTINPUT_VERSION  0x0800

#include <d3dx8.h>
#include <dxerr8.h>
#include <dinput.h>
#include <tchar.h>
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
	CD3DApp( HINSTANCE hInstance, INT ResX, INT ResY );

	// Functions to create, run, pause, and clean up the application
	HRESULT Create();
	HRESULT	Run();

protected:
	virtual ~CD3DApp();

	virtual HRESULT InitDeviceObjects()			= 0;
	virtual HRESULT FrameMove()					= 0;
	virtual HRESULT RestoreDeviceObjects()		= 0;
	virtual HRESULT InvalidateDeviceObjects()	= 0;
	virtual HRESULT Render()					= 0;

	BOOL	bActive;
	BOOL	bReady;

	// DirectD3D
	LPDIRECT3D8				pD3D;
	static LPDIRECT3DDEVICE8		pd3dDevice;

	// DirectInput
	LPDIRECTINPUT8			pDI;				// The main DI object
	LPDIRECTINPUTDEVICE8	pDIDevice;			// The DI device

	// Timer
	static CTimer Timer;

private:
	HRESULT InitD3D();
	HRESULT InitDI();
	HRESULT Render3DEnvironment();
	HRESULT Resize3DEnvironment();

private:
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
