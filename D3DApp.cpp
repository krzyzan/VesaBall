// D3DApp.cpp: implementation of the CD3DApp class.
// ver 1.1
//
//////////////////////////////////////////////////////////////////////

#include "D3DApp.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
CD3DApp* CD3DApp::s_pCurD3DApp = NULL;

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CD3DApp::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return s_pCurD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPDIRECT3DDEVICE8	CD3DApp::pd3dDevice	= NULL;
CTimer				CD3DApp::Timer;

CD3DApp::CD3DApp( HINSTANCE hInstance, INT ResX, INT ResY )
{
	s_pCurD3DApp	= this;

	xRes			= ResX;
	yRes			= ResY;

	bActive			= FALSE;
	bReady			= FALSE;

////////////////////////////

	// Create the Direct3D object
	pD3D = Direct3DCreate8( D3D_SDK_VERSION );

	// Create the DirectInput object
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL); 
	// Initialize mouse input
	InitDI();


    // Register the window class
	WNDCLASS wc = { 0, WndProc, 0, 0, hInstance, NULL, NULL, NULL, NULL,
					  _T("D3D Window") };
	RegisterClass( &wc );

	// Create the render window 
	hWnd = CreateWindow( _T("D3D Window"), _T("VesaBall"), WS_POPUP|WS_SYSMENU|WS_VISIBLE,
						   CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
						   NULL, NULL, hInstance, 0L );

	InitD3D();
}

CD3DApp::~CD3DApp()
{
	bActive = FALSE;
	bReady  = FALSE;

	// Clean up everything and exit the app
	//UnregisterClass( "Ball Game", hInst );

	SAFE_RELEASE( pd3dDevice );
	SAFE_RELEASE( pD3D );
	SAFE_RELEASE( pDIDevice );
	SAFE_RELEASE( pDI );
}

HRESULT CD3DApp::Run()
{
	// Now we're ready to recieve and process Windows messages.
	BOOL bGotMsg;
	MSG  msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if( bActive )
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0U, 0U );

		if( bGotMsg )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Render a frame during idle time (no messages are waiting)
			if( bActive && bReady )
			{
				if( FAILED( Render3DEnvironment() ) )
					SendMessage( hWnd, WM_CLOSE, 0, 0 );
			}
		}
	}

	return (INT)msg.wParam;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CD3DApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
		case WM_ACTIVATE:
			if( pDIDevice ) return InitDI();

		case WM_SETCURSOR:
			// Turn off Windows cursor in fullscreen mode
			if( bActive && bReady )
			{
				SetCursor( NULL );
				return TRUE; // prevent Windows from setting cursor to window class cursor
			}
			break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;

		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
			}
			break;
    }

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT CD3DApp::InitD3D()
{
	// Create the D3D object.
	if (NULL == ( pD3D = Direct3DCreate8( D3D_SDK_VERSION )))
		return E_FAIL;

	// Get the current desktop display mode, so we can set up a back
	// buffer of the same format
	D3DDISPLAYMODE d3ddm;
	if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed							= FALSE;
	d3dpp.SwapEffect						= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat					= d3ddm.Format;
	d3dpp.BackBufferWidth					= xRes;
	d3dpp.BackBufferHeight					= yRes;
	d3dpp.EnableAutoDepthStencil			= TRUE;
	d3dpp.AutoDepthStencilFormat			= D3DFMT_D16;
	d3dpp.BackBufferCount					= 2;
	d3dpp.Flags								= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.FullScreen_RefreshRateInHz		= D3DPRESENT_RATE_DEFAULT;
	d3dpp.FullScreen_PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;

	// Create the D3DDevice
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									  D3DCREATE_HARDWARE_VERTEXPROCESSING,
									  &d3dpp, &pd3dDevice ) ) )
		return E_FAIL;

	// Clear D3DDevice
	pd3dDevice->Present( NULL, NULL, NULL, NULL );


	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Render3DEnvironment()
{
	HRESULT hr;

	// Test the cooperative level to see if it's okay to render
	if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
	{
		// If the device was lost, do not render until we get it back
		if( D3DERR_DEVICELOST == hr )
			return S_OK;

		// Check if the device needs to be reset.
		if( D3DERR_DEVICENOTRESET == hr )
		{
			if( FAILED( hr = Resize3DEnvironment() ) )
				return hr;
		}
		return hr;
	}
	
	// Frame move the scene
	if( FAILED( hr = FrameMove() ) )
		return hr;

	// Render the scene as normal
	if( FAILED( hr = Render() ) )
		return hr;

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

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
	if( FAILED( hr = InvalidateDeviceObjects() ) )
		return hr;

	// Reset the device
	if( FAILED( hr = pd3dDevice->Reset( &d3dpp ) ) )
		return hr;

	// Initialize the app's device-dependent objects
	hr = RestoreDeviceObjects();
	if( FAILED(hr) )
		return hr;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApp::InitDI()
{
	pDI->CreateDevice( GUID_SysMouse, &pDIDevice, NULL );
	pDIDevice->SetDataFormat( &c_dfDIMouse );
	pDIDevice->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	pDIDevice->SetEventNotification( CreateEvent(NULL, FALSE, FALSE, NULL) );

	DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = DIPROPAXISMODE_REL;

	pDIDevice->SetProperty( DIPROP_AXISMODE , &dipdw.diph );
	
	pDIDevice->Acquire(); 

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Create()
// Desc: Create objects thru InitDeviceObjects() and RestoreDeviceObjects()
//-----------------------------------------------------------------------------
HRESULT CD3DApp::Create()
{
	HRESULT hr;
	// Initialize the app's device-dependent objects
 	hr = InitDeviceObjects();
 	if( SUCCEEDED(hr) )
 	{
 		hr = RestoreDeviceObjects();
 		if( SUCCEEDED(hr) )
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
