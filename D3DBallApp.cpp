#include "StdAfx.h"
#include "D3DBallApp.h"
#include "GameMenu.h"
#include "GameEngine.h"
#include "GameEditor.h"


const	MOUSE_BUFFER_SIZE	= 16;
const	KEYBRD_BUFFER_SIZE	= 16;

CD3DBallApp::CD3DBallApp()
{	
	dwFrameMoveCounter		= 0;
	dwFrameRenderCounter	= 0;

	srand( (INT)Timer.GetTime() );
	pScene	= NULL;
}


HRESULT CD3DBallApp::Create( HINSTANCE hInstance )
{
	CD3DApp::Create( hInstance );

	// Create the DirectInput object
	DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL ); 
	// Initialize mouse input
	InitializeMouseInput();
	InitializeKeyboardInput();

	return S_OK;
}

HRESULT CD3DBallApp::InitializeKeyboardInput()
{
    HRESULT hr;

    // Obtain an interface to the system keyboard device.
    if( FAILED( hr = pDI->CreateDevice( GUID_SysKeyboard, &pKeyboardDevice, NULL ) ) )
        return hr;
    
    // Set the data format to "keyboard format" - a predefined data format 
    if( FAILED( hr = pKeyboardDevice->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;
    
    // Set the cooperativity level
    if( FAILED( hr = pKeyboardDevice->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
        return hr;


	DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = KEYBRD_BUFFER_SIZE; // Arbitary buffer size

    if( FAILED( hr = pKeyboardDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
         return hr;

    // Acquire the newly created device
    pKeyboardDevice->Acquire();


    return S_OK;
}


HRESULT CD3DBallApp::InitializeMouseInput()
{
    HRESULT hr;

	if( FAILED( hr = pDI->CreateDevice( GUID_SysMouse, &pMouseDevice, NULL ) ) )
		return hr;

	if( FAILED( hr = pMouseDevice->SetDataFormat( &c_dfDIMouse ) ) )
		return hr;

	if( FAILED( hr = pMouseDevice->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
		return hr;

	if( FAILED( hr = pMouseDevice->SetEventNotification( CreateEvent(NULL, FALSE, FALSE, NULL) ) ) )
		return hr;

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = MOUSE_BUFFER_SIZE; // Arbitary buffer size

    if ( FAILED( hr = pMouseDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
        return hr;

	pMouseDevice->Acquire(); 

	return S_OK;
}


HRESULT CD3DBallApp::InitDeviceObjects()
{
	fTimeToRender = 0;

	pScene = new CGameMenu( pD3DDevice );
	pScene->InitDeviceObjects();

	return S_OK;
}

HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	pScene->RestoreDeviceObjects();

	return S_OK;
}


HRESULT CD3DBallApp::ReadMouseEvents()
{
	DIDEVICEOBJECTDATA didod[ MOUSE_BUFFER_SIZE ];  // Receives buffered data 
	DWORD              dwElements;
	HRESULT            hr;

	if (NULL == pMouseDevice) 
		return S_OK;
	
	dwElements = MOUSE_BUFFER_SIZE;
	if (FAILED( hr = pMouseDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
									didod, &dwElements, 0 ) ) )
		return hr;

	for (DWORD i = 0; i < dwElements; i++) {
		pScene->ProcessMouseEvent( &didod[i] );
	}

	return S_OK;
}


HRESULT CD3DBallApp::ReadKeyboardEvents()
{
	DIDEVICEOBJECTDATA didod[ KEYBRD_BUFFER_SIZE ];  // Receives buffered data 
	DWORD              dwElements;
	HRESULT            hr;

	if( NULL == pKeyboardDevice ) 
		return S_OK;

	dwElements = KEYBRD_BUFFER_SIZE;

	if (FAILED( hr = pKeyboardDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
									didod, &dwElements, 0 ) ) )  
		return hr;

	for (DWORD i = 0; i < dwElements; i++) {
		pScene->ProcessKeybrdEvent( &didod[i] );
	}

	return S_OK;
}


HRESULT CD3DBallApp::FrameMove( float fElapsedTime )
{
	fTimeToRender -= fElapsedTime;
	dwFrameMoveCounter++;

	ReadKeyboardEvents();
	ReadMouseEvents();

	pScene->FrameMove( fElapsedTime );

	CD3DAppScene* pNextScene = pScene->GetNextScene();
	
	if (pNextScene == NULL) {
		SendMessage( hWnd, WM_CLOSE, 0, 0 );
		return S_OK;
	}

	if (pScene != pNextScene ) {
		pScene->InvalidateDeviceObjects();
		pScene->DeleteDeviceObjects();
		SAFE_DELETE( pScene );
		pScene = pNextScene;
		pScene->InitDeviceObjects();
		pScene->RestoreDeviceObjects();
	};


	return S_OK;
}


HRESULT CD3DBallApp::FrameRender()
{
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	dwFrameRenderCounter++;

	pScene->FrameRender();

	// Show the frame on the primary surface.
	pD3DDevice->Present( NULL, NULL, NULL, NULL );


	return S_OK;
}

HRESULT CD3DBallApp::InvalidateDeviceObjects()
{
	pScene->InvalidateDeviceObjects();
	
	return S_OK;
}

HRESULT CD3DBallApp::DeleteDeviceObjects()
{
	pScene->DeleteDeviceObjects();
	SAFE_DELETE( pScene );

	return S_OK;
}


HRESULT	CD3DBallApp::FinalCleanup()
{
	SAFE_RELEASE( pMouseDevice );
	SAFE_RELEASE( pKeyboardDevice );
	SAFE_RELEASE( pDI );

	char str[100] = "FrameMove() / Render(): ";
	char* p = str + strlen(str);
	_gcvt(((float)dwFrameMoveCounter)/dwFrameRenderCounter, 4, p);
	p = str + strlen(str);
	strcat( p, "\nRender() / sec: ");
	p = str + strlen(str);
	_gcvt(dwFrameRenderCounter/Timer.GetTime(), 4, p);

	MessageBox( hWnd, str, "Internal counters", MB_OK );

	return S_OK;
}

