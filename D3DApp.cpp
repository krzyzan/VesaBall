// D3DApp.cpp: implementation of the CD3DApp class.
// v2.00
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "D3DApp.h"

//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
CD3DApp* CD3DApp::s_pD3DApp = NULL;


//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CD3DApp::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return s_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DApp::CD3DApp()
{
	s_pD3DApp		= this;

	bActive			= FALSE;
	bReady			= FALSE;
	CreateFlags		= 0L;

	strWindowTitle	= _T("VesaBall");
	MinDepthBits	= 16;
	MinStencilBits	= 0;
}

HRESULT CD3DApp::Cleanup3DEnvironment()
{
	bActive = FALSE;
	bReady  = FALSE;

	if ( pD3DDevice ) {
        InvalidateDeviceObjects();
		DeleteDeviceObjects();
	}
    
	SAFE_RELEASE( pD3DDevice );
	SAFE_RELEASE( pD3D );

	// Clean up everything and exit the app
	FinalCleanup();

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApp::BuildDeviceList()
{
	const DWORD dwNumDeviceTypes = 2;
	const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF") };
	const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

	BOOL bHALExists	= FALSE;
	BOOL bHALIsDesktopCompatible = FALSE;
	BOOL bHALIsSampleCompatible = FALSE;

	// Fill in adapter info
	D3DAdapterInfo* pAdapter = &Adapter;
	pD3D->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &pAdapter->d3dAdapterIdentifier );
	pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &pAdapter->d3ddmDesktop );
	pAdapter->dwNumDevices = 0;
	pAdapter->dwCurrentDevice = 0;

	// Enumerate all display modes on this adapter
	D3DDISPLAYMODE modes[100];
	D3DFORMAT formats[20];
	DWORD dwNumFormats = 0;
	DWORD dwNumModes = 0;
	DWORD dwNumAdapterModes = pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT );

	// Add the adapter's current desktop format to the list of formats
	formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

	for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
	{
		// Get the display mode attributes
		D3DDISPLAYMODE DisplayMode;
		pD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, iMode, &DisplayMode );

		// Filter out low-resolution modes
		//if ( DisplayMode.Width < 640 || DisplayMode.Height < 400 )
		//	continue;

		// Check if the mode already exists (to filter out refresh rates)
		for( DWORD m=0L; m<dwNumModes; m++ )
		{
			if ( ( modes[m].Width  == DisplayMode.Width	) &&
				( modes[m].Height == DisplayMode.Height ) &&
				( modes[m].Format == DisplayMode.Format ) )
			{
				modes[m].RefreshRate = DisplayMode.RefreshRate;
				break;
			}
		}

		// If we found a new mode, add it to the list of modes
		if ( m == dwNumModes )
		{
			modes[dwNumModes++] = DisplayMode;
			
			// Check if the mode's format already exists
			for( DWORD f=0; f<dwNumFormats; f++ )
			{
				if ( DisplayMode.Format == formats[f] )
					break;
			}

			// If the format is new, add it to the list
			if ( f== dwNumFormats )
				formats[dwNumFormats++] = DisplayMode.Format;
		}
	}

   

	// Add devices to adapter
	for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
	{
		// Fill in device info
		D3DDeviceInfo* pDevice;
		pDevice 				= &pAdapter->devices[pAdapter->dwNumDevices];
		pDevice->DeviceType 	= DeviceTypes[iDevice];
		pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, DeviceTypes[iDevice], &pDevice->d3dCaps );
		pDevice->strDesc		= strDeviceDescs[iDevice];
		pDevice->dwNumModes 	= 0;
		pDevice->dwCurrentMode	= 0;
		pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

		// Examine each format supported by the adapter to see if it will
		// work with this device and meets the needs of the application.
		BOOL  bFormatConfirmed[20];
		DWORD dwBehavior[20];
		D3DFORMAT fmtDepthStencil[20];

		for( DWORD f=0; f<dwNumFormats; f++ )
		{
			bFormatConfirmed[f] = FALSE;
			fmtDepthStencil[f] = D3DFMT_UNKNOWN;

			// Skip formats that cannot be used as render targets on this device
			if ( FAILED( pD3D->CheckDeviceType( D3DADAPTER_DEFAULT, pDevice->DeviceType,
												 formats[f], formats[f], FALSE ) ) )
				continue;

			if ( pDevice->DeviceType == D3DDEVTYPE_HAL )
			{
				// This system has a HAL device
				bHALExists = TRUE;
			}

			// Confirm the device/format for HW vertex processing
			if ( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			{
				if ( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
				{
					dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
									D3DCREATE_PUREDEVICE;
					bFormatConfirmed[f] = TRUE;
				}

				if ( FALSE == bFormatConfirmed[f] )
				{
					dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;
					bFormatConfirmed[f] = TRUE;
				}

				if ( FALSE == bFormatConfirmed[f] )
				{
					dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;
					bFormatConfirmed[f] = TRUE;
				}
			}

			// Confirm the device/format for SW vertex processing
			if ( FALSE == bFormatConfirmed[f] )
			{
				dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				bFormatConfirmed[f] = TRUE;
			}

			// Find a suitable depth/stencil buffer format for this device/format
			if ( bFormatConfirmed[f] )
			{
				if ( !FindDepthStencilFormat( D3DADAPTER_DEFAULT, pDevice->DeviceType,
					formats[f], &fmtDepthStencil[f] ) )
				{
					bFormatConfirmed[f] = FALSE;
				}
			}
		}

		// Add all enumerated display modes with confirmed formats to the
		// device's list of valid modes
		for( DWORD m=0L; m<dwNumModes; m++ )
		{
			for( DWORD f=0; f<dwNumFormats; f++ )
			{
				if ( modes[m].Format == formats[f] )
				{
					if ( bFormatConfirmed[f] == TRUE )
					{
						// Add this mode to the device's list of valid modes
						pDevice->modes[pDevice->dwNumModes].Width		= modes[m].Width;
						pDevice->modes[pDevice->dwNumModes].Height		= modes[m].Height;
						pDevice->modes[pDevice->dwNumModes].Format		= modes[m].Format;
						pDevice->modes[pDevice->dwNumModes].RefreshRate = modes[m].RefreshRate;
						pDevice->modes[pDevice->dwNumModes].dwBehavior	= dwBehavior[f];
						pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
						pDevice->dwNumModes++;

						if ( pDevice->DeviceType == D3DDEVTYPE_HAL )
							bHALIsSampleCompatible = TRUE;
					}
				}
			}
		}

		// Select any RES_X x RES_Y mode for default (but prefer a 32-bit mode)
		for( m=0; m<pDevice->dwNumModes; m++ )
		{
			if ( pDevice->modes[m].Width==RES_X && pDevice->modes[m].Height==RES_Y )
			{
				pDevice->dwCurrentMode = m;

				if ( pDevice->modes[m].Format == D3DFMT_X8R8G8B8) break;

				if ( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
					pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
					pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
				{
					break;
				}
			}
		}

		// If valid modes were found, keep this device
		if ( pDevice->dwNumModes > 0 )
			pAdapter->dwNumDevices++;
	}

	// Return an error if no compatible devices were found
	if ( 0L == pAdapter->dwNumDevices )
		return D3DAPPERR_NOCOMPATIBLEDEVICES;

	// Pick a default device that can render into a window
	// (This code assumes that the HAL device comes before the REF
	// device in the device array).
	Adapter.dwCurrentDevice = 0;

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Run()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Run()
{
	// Now we're ready to recieve and process Windows messages.
	BOOL bGotMsg;
	MSG  msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while (WM_QUIT != msg.message)
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if ( bActive )
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

		if ( bGotMsg )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Render a frame during idle time (no messages are waiting)
			if ( bActive && bReady )
			{
				if ( FAILED( Render3DEnvironment() ) )
					SendMessage( hWnd, WM_CLOSE, 0, 0 );
			}
		}
	}

	return (INT)msg.wParam;
}


//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//		 with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CD3DApp::FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
	D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
	if ( MinDepthBits <= 16 && MinStencilBits == 0 )
	{
		if ( SUCCEEDED( pD3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
		{
			if ( SUCCEEDED( pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D16;
				return TRUE;
			}
		}
	}

	if ( MinDepthBits <= 15 && MinStencilBits <= 1 )
	{
		if ( SUCCEEDED( pD3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
		{
			if ( SUCCEEDED( pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D15S1;
				return TRUE;
			}
		}
	}

	if ( MinDepthBits <= 24 && MinStencilBits == 0 )
	{
		if ( SUCCEEDED( pD3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
		{
			if ( SUCCEEDED( pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D24X8;
				return TRUE;
			}
		}
	}

	if ( MinDepthBits <= 24 && MinStencilBits <= 8 )
	{
		if ( SUCCEEDED( pD3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
		{
			if ( SUCCEEDED( pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D24S8;
				return TRUE;
			}
		}
	}

	if ( MinDepthBits <= 24 && MinStencilBits <= 4 )
	{
		if ( SUCCEEDED( pD3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
		{
			if ( SUCCEEDED( pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D24X4S4;
				return TRUE;
			}
		}
	}

	if ( MinDepthBits <= 32 && MinStencilBits == 0 )
	{
		if ( SUCCEEDED( pD3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
		{
			if ( SUCCEEDED( pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D32;
				return TRUE;
			}
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CD3DApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
									LPARAM lParam )
{
	switch( uMsg )
	{
  		case WM_SETCURSOR:
			// Turn off Windows cursor in fullscreen mode
			if ( bActive && bReady )
			{
				SetCursor( NULL );
				return TRUE; // prevent Windows from setting cursor to window class cursor
			}
			break;

		case WM_POWERBROADCAST:
			switch( wParam )
			{
				#ifndef PBT_APMQUERYSUSPEND
					#define PBT_APMQUERYSUSPEND 0x0000
				#endif
				case PBT_APMQUERYSUSPEND:
					// At this point, the app should save any data for open
					// network connections, files, etc., and prepare to go into
					// a suspended mode.
					return TRUE;

				#ifndef PBT_APMRESUMESUSPEND
					#define PBT_APMRESUMESUSPEND 0x0007
				#endif
				case PBT_APMRESUMESUSPEND:
					// At this point, the app should recover any data, network
					// connections, files, etc., and resume running from when
					// the app was suspended.
					return TRUE;
			}
			break;

		case WM_SYSCOMMAND:
			// Prevent moving/sizing and power loss in fullscreen mode
			switch( wParam )
			{
				case SC_MOVE:
				case SC_SIZE:
				case SC_MAXIMIZE:
				case SC_KEYMENU:
				case SC_MONITORPOWER:
				  return 1;
					break;
			}
			break;

		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_ESCAPE:
					SendMessage( hWnd, WM_CLOSE, 0, 0 );
					break;
			}
			break;

		case WM_CLOSE:
			Cleanup3DEnvironment();
			DestroyWindow( hWnd );
            PostQuitMessage(0);
            return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Initialize3DEnvironment()
{

	HRESULT hr;

	D3DAdapterInfo* pAdapterInfo = &Adapter;
	D3DDeviceInfo*	pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo*	pModeInfo	 = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	// Set up the presentation parameters
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed							= FALSE;
	d3dpp.BackBufferCount					= 2;
	d3dpp.MultiSampleType					= pDeviceInfo->MultiSampleType;
	d3dpp.SwapEffect						= D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil			= TRUE;
	d3dpp.AutoDepthStencilFormat			= pModeInfo->DepthStencilFormat;
	d3dpp.hDeviceWindow						= hWnd;
	d3dpp.BackBufferWidth					= pModeInfo->Width;
	d3dpp.BackBufferHeight					= pModeInfo->Height;
	d3dpp.BackBufferFormat					= pModeInfo->Format;
	d3dpp.FullScreen_RefreshRateInHz		= pModeInfo->RefreshRate;
	d3dpp.FullScreen_PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;


	
	// Create the device
	hr = pD3D->CreateDevice( 0, pDeviceInfo->DeviceType,
							   hWnd, pModeInfo->dwBehavior, &d3dpp,
							   &pD3DDevice );

	if ( SUCCEEDED(hr) )
	{
		// Clear the viewport
		pD3DDevice->Present( NULL, NULL, NULL, NULL);

		// When moving from fullscreen to windowed mode, it is important to
		// adjust the window size after recreating the device rather than
		// beforehand to ensure that you get the window size you want.	For
		// example, when switching from 640x480 fullscreen to windowed with
		// a 1000x600 window on a 1024x768 desktop, it is impossible to set
		// the window size to 1000x600 until after the display mode has
		// changed to 1024x768, because windows cannot be larger than the
		// desktop.

		// Store device Caps
		pD3DDevice->GetDeviceCaps( &d3dCaps );
		CreateFlags = pModeInfo->dwBehavior;

		return S_OK;
	}

	return hr;
}

//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Render3DEnvironment()
{
	HRESULT hr;

	// Test the cooperative level to see if it's okay to render
	if ( FAILED( hr = pD3DDevice->TestCooperativeLevel() ) )
	{
		// If the device was lost, do not render until we get it back
		if ( D3DERR_DEVICELOST == hr )
			return S_OK;

		// Check if the device needs to be reset.
		if ( D3DERR_DEVICENOTRESET == hr )
		{
			if ( FAILED( hr = Resize3DEnvironment() ) )
				return hr;
		}
		return hr;
	}

	// Render the scene as normal
	if ( FAILED( hr = FrameMove( Timer.GetElapsedTime() ) ) )
		return hr;

	// Render the scene as normal
	if ( FAILED( hr = FrameRender() ) )
		return hr;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Resize3DEnvironment()
{
	HRESULT hr;

	// Release all vidmem objects
	if ( FAILED( hr = InvalidateDeviceObjects() ) )
		return hr;

	// Reset the device
	if ( FAILED( hr = pD3DDevice->Reset( &d3dpp ) ) )
		return hr;

	// Initialize the app's device-dependent objects
	hr = RestoreDeviceObjects();
	if ( FAILED(hr) )
		return hr;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Create( HINSTANCE hInstance )
{
	HRESULT hr;
	// Create the Direct3D object
	pD3D = Direct3DCreate8( D3D_SDK_VERSION );

	// Build a list of Direct3D adapters, modes and devices.
	BuildDeviceList();


	// Register the windows class
	//WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
	//					  LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
	//					  LoadCursor( NULL, IDC_ARROW ),
	//					  (HBRUSH)GetStockObject(WHITE_BRUSH), 
	//					  NULL, strWindowTitle };
	WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
						  NULL, NULL, NULL, 
						  NULL, strWindowTitle };
	RegisterClass( &wndClass );

	// Create the render window 
	hWnd = CreateWindow( strWindowTitle, strWindowTitle, 
			WS_POPUP|WS_SYSMENU|WS_VISIBLE,	
			CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, 0L );

	// Initialize the 3D environment for the app
	Initialize3DEnvironment();

	// Initialize the app's device-dependent objects
 	hr = InitDeviceObjects();
 	if ( SUCCEEDED(hr) )
 	{
 		hr = RestoreDeviceObjects();
 		if ( SUCCEEDED(hr) )
 		{
 			Timer.Start();
 			// The app is ready to go
 			bActive = TRUE;
 			bReady = TRUE;
 
 			return S_OK;
 		}
 	}
 
 	return hr;
}