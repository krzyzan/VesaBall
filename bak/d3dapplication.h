// D3DApp.h: interface for the CD3DApp class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#define DIRECTINPUT_VERSION  0x0800

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>

#include <D3D8.h>
#include <dinput.h>
#include "timer.h"

#include "resource.h"


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


//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


class CD3DApplication 
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



protected:
	CD3DApplication( HINSTANCE hInstance );

	// Internal variables for the state of the app
	D3DAdapterInfo			m_Adapter;
	BOOL					bActive;
	BOOL					bReady;

	TCHAR	FrameStatsText[90];
	TCHAR	DeviceStatsText[90];


	// Overridable variables for the app
	TCHAR*					strWindowTitle;		// Title for the app's window
	DWORD					MinDepthBits;		// Minimum number of bits needed in depth buffer
	DWORD					MinStencilBits;		// Minimum number of bits needed in stencil buffer

	// Variables for timing
	FLOAT					m_fTime;				// Current time in seconds
	FLOAT					m_fElapsedTime;			// Time elapsed since last frame
	FLOAT					fFPS; 				// Instanteous frame rate

	// Main objects used for creating and rendering the 3D scene
	D3DPRESENT_PARAMETERS	m_d3dpp;				// Parameters for CreateDevice/Reset
	HWND					hWnd; 				// The main app window
	LPDIRECT3D8 			D3D; 				// The main D3D object
	D3DCAPS8				m_d3dCaps;				// Caps for the device
	D3DSURFACE_DESC 		m_d3dsdBackBuffer;		// Surface desc of the backbuffer
	DWORD					CreateFlags;		// Indicate sw or hw vertex processing

	// DirectInput
	LPDIRECTINPUT8			DI;					// The main DI object
	LPDIRECTINPUTDEVICE8	DIDevice;			// The DI device

private:

	// Internal functions to manage and render the 3D scene
	HRESULT BuildDeviceList();
	BOOL	FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
				D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );
	
	HRESULT Initialize3DEnvironment();
	HRESULT Render3DEnvironment();
	HRESULT Resize3DEnvironment();

	//Internal functions to manage DirectInput
	HRESULT InitializeMouseInput();

protected:
	virtual ~CD3DApplication();

	virtual HRESULT InitDeviceObjects()			= 0;
	virtual HRESULT FrameMove()					= 0;
	virtual HRESULT RestoreDeviceObjects()		= 0;
	virtual HRESULT InvalidateDeviceObjects()	= 0;
	virtual HRESULT Render()					= 0;

public:
	// Functions to create, run, pause, and clean up the application
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HRESULT Create();
	INT 	Run();

protected:
	static LPDIRECT3DDEVICE8	pd3dDevice;
	static CTimer				Timer;


};
