// D3DApp.h: interface for the CD3DApp class.
// v1.11
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v1.00
	- Wersja poczatkowa

v1.10
	- Obs³uga Alt-Tab
	- Obs³uga myszki (Direct Input)
	- Czyszczenie ekranu na starcie
	- Wiêcej sprawdzania zwracanych kodów b³êdu (SUCCEEDED, FAILED)
	- Zamiana globalnych zmiennych i funkcji na prywatne statyczne (¿eby ich nikt nie rusza³)

v1.11
	- Poprawiona obs³uga myszki (by³ z³y format danych)
	- Usuniêty memory leak zwi¹zany z obiektem D3D
	- Nowy CTimer, dzia³a na komputerach, które nie obs³uguj¹ QueryPerformanceCounter()

v2.00
	- "Inteligentna" enumeracja trybów graficznych obs³ugiwanych przez kartê
	- Wybór najwy¿szego dostepnego odœwie¿ania (wym. monitor Plug & Play)
*/

#pragma once
#include <d3dx8.h>
#include "timer.h"

//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------

enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D		  0x82000001
#define D3DAPPERR_NOWINDOW			  0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE	  0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE	  0x82000006
#define D3DAPPERR_NOWINDOWEDHAL 	  0x82000007
#define D3DAPPERR_NODESKTOPHAL		  0x82000008
#define D3DAPPERR_NOHALTHISMODE 	  0x82000009
#define D3DAPPERR_NONZEROREFCOUNT	  0x8200000a
#define D3DAPPERR_MEDIANOTFOUND 	  0x8200000b
#define D3DAPPERR_RESIZEFAILED		  0x8200000c

class CD3DApp 
{

	//-----------------------------------------------------------------------------
	// Name: struct D3DModeInfo
	// Desc: Structure for holding information about a display mode
	//-----------------------------------------------------------------------------
	struct D3DModeInfo
	{
		DWORD	   Width;				// Screen width in this mode
		DWORD	   Height;				// Screen height in this mode
		D3DFORMAT  Format;				// Pixel format in this mode
		DWORD	   RefreshRate;
		DWORD	   dwBehavior;			// Hardware / Software / Mixed vertex processing
		D3DFORMAT  DepthStencilFormat;	// Which depth/stencil format to use with this mode
	};

	//-----------------------------------------------------------------------------
	// Name: struct D3DDeviceInfo
	// Desc: Structure for holding information about a Direct3D device, including
	//		 a list of modes compatible with this device
	//-----------------------------------------------------------------------------
	struct D3DDeviceInfo
	{
		// Device data
		D3DDEVTYPE	 DeviceType;	  // Reference, HAL, etc.
		D3DCAPS8	 d3dCaps;		  // Capabilities of this device
		const TCHAR* strDesc;		  // Name of this device

		// Modes for this device
		DWORD		 dwNumModes;
		D3DModeInfo  modes[150];

		// Current state
		DWORD		 dwCurrentMode;
		D3DMULTISAMPLE_TYPE MultiSampleType;
	};


	//-----------------------------------------------------------------------------
	// Name: struct D3DAdapterInfo
	// Desc: Structure for holding information about an adapter, including a list
	//		 of devices available on this adapter
	//-----------------------------------------------------------------------------
	struct D3DAdapterInfo
	{
		// Adapter data
		D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
		D3DDISPLAYMODE d3ddmDesktop;	  // Desktop display mode for this adapter

		// Devices for this adapter
		DWORD		   dwNumDevices;
		D3DDeviceInfo  devices[5];

		// Current state
		DWORD		   dwCurrentDevice;
	};

public:
	CD3DApp();

	// Functions to create, run, pause, and clean up the application
	HRESULT Create( HINSTANCE hInstance );
	HRESULT Run();

protected:
	virtual HRESULT InitDeviceObjects()				= 0;
	virtual HRESULT RestoreDeviceObjects()			= 0;
	virtual HRESULT FrameMove( float fElapsedTime )	= 0;
	virtual HRESULT FrameRender()					= 0;
	virtual HRESULT InvalidateDeviceObjects()		= 0;
	virtual HRESULT DeleteDeviceObjects()			= 0;
	virtual HRESULT FinalCleanup()					= 0;

	// Internal variables for the state of the app
	BOOL					bActive;
	BOOL					bReady;

	// Direct3D
	LPDIRECT3D8 			pD3D; 			// The main D3D object
	LPDIRECT3DDEVICE8		pD3DDevice;		// The main D3D object

	// Timer
	CTimer					Timer;

private:
	// Internal functions to manage and render the 3D scene
	HRESULT BuildDeviceList();
	BOOL	FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
				D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );

	HRESULT Initialize3DEnvironment();
	HRESULT Render3DEnvironment();
	HRESULT Resize3DEnvironment();
	HRESULT	Cleanup3DEnvironment();

private:
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// Main objects used for creating and rendering the 3D scene
	D3DPRESENT_PARAMETERS	d3dpp;				// Parameters for CreateDevice/Reset
	D3DAdapterInfo			Adapter;
	D3DCAPS8				d3dCaps;			// Caps for the device
	DWORD					CreateFlags;		// Indicate sw or hw vertex processing

	// Overridable variables for the app
	TCHAR*					strWindowTitle;		// Title for the app's window
	DWORD					MinDepthBits;		// Minimum number of bits needed in depth buffer
	DWORD					MinStencilBits;		// Minimum number of bits needed in stencil buffer

protected:
	HWND		hWnd; 							// The main app window

	// Needed for the static WndProc()
	static CD3DApp* s_pD3DApp;
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};
