#include "StdAfx.h"
#include "D3DApp.h"

CD3DApp* CD3DApp::spD3DApp = NULL;

LRESULT CALLBACK CD3DApp::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return spD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}


CD3DApp::CD3DApp( TCHAR* WindowTitle )
{
	spD3DApp		= this;

	bActive			= false;
	bReady			= false;

	CreateFlags		= 0L;

	strWindowTitle	= WindowTitle;
	MinDepthBits	= 16;
	MinStencilBits	= 0;
}


HRESULT CD3DApp::Create( HINSTANCE hInstance )
{
	HRESULT hr;
	
	pD3D = Direct3DCreate8( D3D_SDK_VERSION );

	if (FAILED( hr = BuildDeviceList() ) )
		return hr;

	WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
						  NULL, NULL, NULL, 
						  NULL, strWindowTitle };
	RegisterClass( &wndClass );

	hWnd = CreateWindow( strWindowTitle, strWindowTitle, 
			WS_POPUP|WS_SYSMENU|WS_VISIBLE,	
			CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, 0L );

	if (FAILED( hr = Initialize3DEnvironment() ) )
		return hr;

	if (FAILED( hr = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL ) ) )
		return hr;

	if (FAILED( hr = InitializeMouseInput() ) )
		return hr;

	if (FAILED( hr = InitializeKeyboardInput() ) )
		return hr;

	srand( (DWORD)Timer.GetAbsoluteTime() );

	Timer.Start();

	bActive = TRUE;
	bReady = TRUE;

	return S_OK;
}


HRESULT CD3DApp::Run()
{
	BOOL bGotMsg;
	MSG  msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while (WM_QUIT != msg.message) {
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if ( bActive )
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

		if ( bGotMsg ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else {
			if ( bActive && bReady ) {
				if ( FAILED( Render3DEnvironment() ) )
					SendMessage( hWnd, WM_CLOSE, 0, 0 );
			}
		}
	}

	return (INT)msg.wParam;
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
			//if (wParam == VK_BACK) {
			//	SendMessage( hWnd, WM_CLOSE, 0, 0 );//TODO: TMP
			//}
			break;

		case WM_CLOSE:
			Cleanup3DEnvironment();
			DestroyWindow( hWnd );
            PostQuitMessage(0);
            return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


HRESULT CD3DApp::Initialize3DEnvironment()
{
	HRESULT hr;

	D3DAdapterInfo* pAdapterInfo = &Adapter;
	D3DDeviceInfo*	pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo*	pModeInfo	 = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	// Ustaw tryb graficzny oraz atrybuty prezentacji
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed							= FALSE;
	d3dpp.BackBufferCount					= 1;
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


	// Utwórz urz¹dzenie D3D
	hr = pD3D->CreateDevice( 0, pDeviceInfo->DeviceType,
							   hWnd, pModeInfo->dwBehavior, &d3dpp,
							   &pD3DDevice );

	if ( FAILED( hr ) )
		return hr;
	
	// Czyœci ekran
	pD3DDevice->Present( NULL, NULL, NULL, NULL);

	// Zapisz mo¿liwoœci urz¹dzenia
	pD3DDevice->GetDeviceCaps( &d3dCaps );
	CreateFlags = pModeInfo->dwBehavior;

	// Ustaw urz¹dzenie D3D które uzywaj¹ obiekty CD3DScene
	CD3DScene::pD3DDevice = pD3DDevice;

	return S_OK;
}


HRESULT CD3DApp::Render3DEnvironment()
{
	HRESULT hr;

	// Testuje czy mo¿na renderowaæ
	if ( FAILED( hr = pD3DDevice->TestCooperativeLevel() ) ) {
		// Jeœli D3DERR_DEVICELOST, nie renderuj dopóki nie odzyskamy urz¹dzenia.
		if ( D3DERR_DEVICELOST == hr )
			return S_OK;

		// SprawdŸ czy urzadzenie musi byæ zresetowane
		if ( D3DERR_DEVICENOTRESET == hr ) {
			if ( FAILED( hr = Reset3DEnvironment() ) )
				return hr;
		}
		return hr;
	}

	// G³ówna pêtla programu
	float fElapsedTime = Timer.GetElapsedTime();

	if ( FAILED( hr = ReadKeyboardEvents() ) )
		return hr;

	if ( FAILED( hr = ReadMouseEvents() ) )
		return hr;

	if ( FAILED( hr = sD3DScenes.top()->FrameMove( fElapsedTime ) ) )
		return hr;

	if ( FAILED( hr = pD3DDevice->BeginScene() ) )
		return hr;

	if ( FAILED( hr = sD3DScenes.top()->FrameRender() ) )
		return hr;

	if ( FAILED( hr = pD3DDevice->EndScene() ) )
		return hr;

	if ( FAILED( hr = pD3DDevice->Present( NULL, NULL, NULL, NULL ) ) )
		return hr;

	if ( FAILED( hr = ChangeScene() ) )
		return hr;

	return S_OK;
}

HRESULT CD3DApp::Reset3DEnvironment()
{
	HRESULT hr;

	// Zwolnij wszystkie obiekty karty w aktualnej scenie
	if ( FAILED( hr = sD3DScenes.top()->InvalidateDeviceObjects() ) )
		return hr;

	// Zresetuj urz¹dzenie
	if ( FAILED( hr = pD3DDevice->Reset( &d3dpp ) ) )
		return hr;

	// Inicjalizuj wszystkie obiekty karty w aktualnej scenie
	if ( FAILED( hr = sD3DScenes.top()->RestoreDeviceObjects() ) )
		return hr;

	return S_OK;
}


HRESULT CD3DApp::Cleanup3DEnvironment()
{
	// Wyczyœæ wszystko i wyjdŸ z aplikacji
	bActive = FALSE;
	bReady  = FALSE;

	if ( pD3DDevice ) {
		if (!sD3DScenes.empty())
			sD3DScenes.top()->InvalidateDeviceObjects();
		while (!sD3DScenes.empty()) {
			sD3DScenes.top()->DeleteDeviceObjects();
			delete sD3DScenes.top();
			sD3DScenes.pop();
		}
	}

	SAFE_RELEASE( pDIMouse );
	SAFE_RELEASE( pDIKeyboard );
	SAFE_RELEASE( pDI );

	SAFE_RELEASE( pD3DDevice );
	SAFE_RELEASE( pD3D );

	return S_OK;
}


HRESULT CD3DApp::InitializeKeyboardInput()
{
    HRESULT hr;

    // Pobierz interfejs klawiatury
    if( FAILED( hr = pDI->CreateDevice( GUID_SysKeyboard, &pDIKeyboard, NULL ) ) )
        return hr;
    
    // Ustaw format danych na predefiowany format klawiatury
    if( FAILED( hr = pDIKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;
    
    if( FAILED( hr = pDIKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
        return hr;

	// W³¹cz buforowany odczyt z urz¹dzenia
	DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = KEYBRD_BUFFER_SIZE;

    if( FAILED( hr = pDIKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
         return hr;

    // Pobierz nowo utworzony interfejs
	if( FAILED( hr = pDIKeyboard->Acquire() ) )
		return hr;

    return S_OK;
}


HRESULT CD3DApp::InitializeMouseInput()
{
    HRESULT hr;

	// Pobierz interfejs myszki
	if( FAILED( hr = pDI->CreateDevice( GUID_SysMouse, &pDIMouse, NULL ) ) )
		return hr;

	// Ustaw format danych na predefiowany format myszki
	if( FAILED( hr = pDIMouse->SetDataFormat( &c_dfDIMouse ) ) )
		return hr;

	if( FAILED( hr = pDIMouse->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
		return hr;

	if( FAILED( hr = pDIMouse->SetEventNotification( CreateEvent(NULL, FALSE, FALSE, NULL) ) ) )
		return hr;

	// W³¹cz buforowany odczyt z urz¹dzenia
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = MOUSE_BUFFER_SIZE;

    if ( FAILED( hr = pDIMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
        return hr;

    // Pobierz nowo utworzony interfejs
	if( FAILED( hr = pDIMouse->Acquire() ) )
		return hr;

	return S_OK;
}


HRESULT CD3DApp::ReadMouseEvents()
{
	DIDEVICEOBJECTDATA didod[ MOUSE_BUFFER_SIZE ]; 
	DWORD              dwElements;
	HRESULT            hr;

	if (NULL == pDIMouse) 
		return S_OK;
	
	dwElements = MOUSE_BUFFER_SIZE;
	if (FAILED( hr = pDIMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 ) ) )
		return hr;

	for (DWORD i = 0; i < dwElements; i++) {
		if (FAILED( sD3DScenes.top()->ProcessMouseEvent( &didod[i] ) ) )
			return hr;
	}

	return S_OK;
}


HRESULT CD3DApp::ReadKeyboardEvents()
{
	DIDEVICEOBJECTDATA didod[ KEYBRD_BUFFER_SIZE ];
	DWORD              dwElements;
	HRESULT            hr;

	if( NULL == pDIKeyboard ) 
		return S_OK;

	dwElements = KEYBRD_BUFFER_SIZE;
	if (FAILED( hr = pDIKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 ) ) )
		return hr;

	for (DWORD i = 0; i < dwElements; i++)
		if (FAILED( sD3DScenes.top()->ProcessKeybrdEvent( &didod[i] ) ) )
			return hr;

	return S_OK;
}


HRESULT CD3DApp::StartNewScene( CD3DScene* pScene )
{
	HRESULT hr;

	if (!sD3DScenes.empty())
		if (FAILED( hr = sD3DScenes.top()->InvalidateDeviceObjects() ) )
			return hr;

	sD3DScenes.push( pScene );
	
 	if (FAILED( hr = sD3DScenes.top()->InitDeviceObjects() ) )
		return hr;

 	if (FAILED( hr = sD3DScenes.top()->RestoreDeviceObjects() ) )
		return hr;

	return S_OK;
}

HRESULT CD3DApp::StartParentScene()
{
	HRESULT hr;

	if (FAILED( hr = sD3DScenes.top()->InvalidateDeviceObjects() ) )
		return hr;

	if (FAILED( sD3DScenes.top()->DeleteDeviceObjects() ) )
		return hr;

	delete sD3DScenes.top();
	sD3DScenes.pop();

	if (sD3DScenes.empty()) {
		SendMessage( hWnd, WM_CLOSE, 0, 0 );
		return S_OK;
	}

	if (FAILED( sD3DScenes.top()->RestoreDeviceObjects() ) )
		return hr;

	return S_OK;
}


HRESULT CD3DApp::ChangeScene()
{
	CD3DScene* pCurrentScene = sD3DScenes.top()->GetNextScene();

	if (pCurrentScene == NULL)
		return StartParentScene();
	
	if (pCurrentScene != sD3DScenes.top())
		return StartNewScene( pCurrentScene );

	return S_OK;
}
