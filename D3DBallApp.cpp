// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v0.18
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DBallApp.h"
#include "GameMenu.h"
#include "Level.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD3DBallApp::CD3DBallApp()
{	
	srand( (INT)Timer.GetTime() );
	numFrameMove	= 0;
	numRender		= 0;
	pScene			= NULL;
}

HRESULT CD3DBallApp::InitDeviceObjects()
{
	pScene = new CGameMenu( pd3dDevice, pDIDevice );
	pScene->InitDeviceObjects();

	timerFrameMove.Start();
	fTimeToRender = 0;

	return S_OK;
}

HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	pScene->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CD3DBallApp::FrameMove()
{
	FLOAT fElapsedTime = timerFrameMove.GetElapsedTime();
	fTimeToRender -= fElapsedTime;

	// TMP: nie wiem czemu u mnie na starcie timer zawiesza siê na 2 sek. 
	// mo¿e czas wymieniæ BIOS??? :(
	if ( fElapsedTime > 0.1 ) return S_OK;	
	numFrameMove++;

	if ( FAILED( pScene->FrameMove( fElapsedTime ) ) ) {
		pScene->InvalidateDeviceObjects();
		pScene->DeleteDeviceObjects();
		delete pScene;
		
		pScene = new CLevel( pd3dDevice, pDIDevice );
		pScene->InitDeviceObjects();
		pScene->RestoreDeviceObjects();
	}

	return S_OK;
}


HRESULT CD3DBallApp::Render()
{
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	numRender++;

	pScene->Render();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

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
	delete pScene;
	return S_OK;
}

HRESULT	CD3DBallApp::FinalCleanup()
{
	char str[100] = "FrameMove() / Render(): ";
	char* p = str + strlen(str);
	_gcvt(((FLOAT)numFrameMove)/numRender, 4, p);
	p = str + strlen(str);
	strcat( p, "\nRender() / sec: ");
	p = str + strlen(str);
	_gcvt(numRender/Timer.GetTime(), 4, p);

	MessageBox( hWnd, str, 
		"Internal counters", MB_OK );

	return S_OK;
}
