// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v0.18
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DBallApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD3DBallApp::CD3DBallApp()
{	
	srand( (INT)Timer.GetTime() );
	numFrameMove	= 0;
	numRender		= 0;
	pLevel			= NULL;
}

HRESULT CD3DBallApp::InitDeviceObjects()
{
	pLevel = new CLevel( pd3dDevice, pDIDevice );
	pLevel->InitDeviceObjects();

	timerFrameMove.Start();
	fTimeToRender = 0;

	return S_OK;
}

HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	pLevel->RestoreDeviceObjects();

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

	return pLevel->FrameMove( fElapsedTime );
}


HRESULT CD3DBallApp::Render()
{
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	numRender++;

	//pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	pLevel->Render();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}

HRESULT CD3DBallApp::InvalidateDeviceObjects()
{
	pLevel->InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CD3DBallApp::DeleteDeviceObjects()
{
	pLevel->DeleteDeviceObjects();
	delete pLevel;
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
