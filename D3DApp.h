#ifndef D3DAPP_H
#define D3DAPP_H

#include <d3dx8.h>
#include <stack>
using namespace std;

#include "D3DScene.h"
#include "Timer.h"


//TODO: get rid of this

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

const int MOUSE_BUFFER_SIZE		= 16;
const int KEYBRD_BUFFER_SIZE	= 16;


//! Direct3D application
/*!
	Simplifies working with Direct3D. Handles initializing the D3D interface and enumerating the available graphics modes.
	Manages #CD3DScene objects, performing their initialization, reset, and shutdown in the correct order.
*/

class CD3DApp 
{
	// Structure holding information about a graphics mode.
	struct D3DModeInfo
	{
		DWORD	   Width;				// Horizontal resolution in pixels
		DWORD	   Height;				// Vertical resolution in pixels
		D3DFORMAT  Format;				// Pixel format
		DWORD	   RefreshRate;
		DWORD	   dwBehavior;			// Vertex processing: Hardware / Software / Mixed
		D3DFORMAT  DepthStencilFormat;	// Which depth/stencil buffer format to use with this mode
	};

	// Structure holding information about a Direct3D device, 
	// along with the list of modes compatible with this device.
	struct D3DDeviceInfo
	{
		// Device data
		D3DDEVTYPE	 DeviceType;	  // Reference, HAL, etc.
		D3DCAPS8	 d3dCaps;		  // Device capabilities
		const TCHAR* strDesc;		  // Device name

		// Graphics modes for this device
		DWORD		 dwNumModes;
		D3DModeInfo  modes[150];

		// Current state
		DWORD		 dwCurrentMode;
		D3DMULTISAMPLE_TYPE MultiSampleType;
	};


	// Structure holding information about a graphics card, 
	// along with the list of Direct3D-compatible devices available on this card.
	struct D3DAdapterInfo
	{
		// Graphics card data
		D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
		D3DDISPLAYMODE d3ddmDesktop;	  // Desktop display mode for this adapter

		// Devices on this graphics card
		DWORD		   dwNumDevices;
		D3DDeviceInfo  devices[5];

		// Current state
		DWORD		   dwCurrentDevice;
	};

public:
		//! Constructor
		/*!
			\param	WindowTitle	Application window title
		*/
	CD3DApp( TCHAR* WindowTitle );

		//! Creates the Direct3D application
		/*!
			- Creates the application window.
			- Enumerates and selects a suitable mode supported by the graphics card.
			- Initializes asynchronous (buffered) reading from the keyboard and mouse.
			- Starts the internal timer
			- Initializes the random number generator
		*/
	HRESULT Create( HINSTANCE hInstance );

		//! Sets the current scene
		/*!
			Should be called after #Create() has executed;
		*/
	HRESULT StartNewScene( CD3DScene* pScene );

		//! Runs the main program loop
		/*!
			Handles retrieving and processing Windows events.
			If there are no events, renders the current scene.
		*/
	HRESULT Run();

private:
	// Internal functions managing and rendering 3D scenes
	HRESULT	ChangeScene();
	HRESULT StartParentScene();

	HRESULT ProcessMouseEvents();
	HRESULT ProcessKeyboardEvents();

	// DirectInput
	HRESULT InitializeMouseInput();
	HRESULT InitializeKeyboardInput();

	HRESULT Initialize3DEnvironment();
	HRESULT Render3DEnvironment();
	HRESULT Reset3DEnvironment();
	HRESULT	Cleanup3DEnvironment();

	// Internal functions enumerating card modes
	HRESULT BuildDeviceList();
	BOOL	FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
				D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );

	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	// Internal variables describing the application's state
	bool					bActive;
	bool					bReady;

	// Scene stack
	stack<CD3DScene*>		sD3DScenes;

	// Direct3D
	LPDIRECT3D8 			pD3D;
	LPDIRECT3DDEVICE8		pD3DDevice;

	// DirectInput
	LPDIRECTINPUT8			pDI;
	LPDIRECTINPUTDEVICE8	pDIMouse;
	LPDIRECTINPUTDEVICE8	pDIKeyboard;

	// Main timer
	CTimer					Timer;

private:
	// Objects used to create and render the 3D scene

	// Parameters for CreateDevice/Reset
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DAdapterInfo			Adapter;
	
	// Device capabilities
	D3DCAPS8	d3dCaps;

	// Indicates hardware/software vertex processing
	DWORD		CreateFlags;

	// Minimum number of bits in the depth buffer
	DWORD		MinDepthBits;	
	// Minimum number of bits in the stencil buffer
	DWORD		MinStencilBits;	

	// Application window title
	TCHAR*		strWindowTitle;

private:
	// Main application window
	HWND		hWnd;

	// Static pointer to the application, needed for the static WndProc()
	static CD3DApp* spD3DApp;

	// Static event handler, forwards events to the spD3DApp application.
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif
