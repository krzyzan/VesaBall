// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v0.45
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DBallApp.h"
#include "GameMenu.h"
#include "Level.h"


CD3DBallApp::CD3DBallApp()
{	
	srand( (INT)Timer.GetTime() );
	pScene	= NULL;
}

HRESULT CD3DBallApp::InitDeviceObjects()
{
	pScene = new CGameMenu( pd3dDevice, pDIDevice );
	pScene->InitDeviceObjects();

	return S_OK;
}

HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	pScene->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CD3DBallApp::RenderLoop()
{
	switch ( pScene->RenderLoop() ) {
		case CGameMenu::START:
			pScene->InvalidateDeviceObjects();
			pScene->DeleteDeviceObjects();
			SAFE_DELETE( pScene );
			pScene = new CLevel( hWnd, pd3dDevice, pDIDevice );
			pScene->InitDeviceObjects();
			pScene->RestoreDeviceObjects();
			break;
		case CGameMenu::QUIT:
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		case E_FAIL:
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
	}

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
	return S_OK;
}

