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
	pSprite			= NULL;
	ZeroMemory( pTex, sizeof(pTex) );
}

HRESULT CD3DBallApp::InitDeviceObjects()
{
	LoadTexture( "gfx/Bg_tree.jpg",			0 );
	LoadTexture( "gfx/Paddle.png",			40 );
	LoadTexture( "gfx/SparkEffect.png",		50 );
	LoadTexture( "gfx/Brick1.png",			51 );
	LoadTexture( "gfx/Brick6.png",			52 );
	LoadTexture( "gfx/Bonus_Fireball.png",	60 );
	LoadTexture( "gfx/Ball_alu.png",		70 );

	pLevel = new CLevel();

	//Tworzymy t³o
	new CSprite( pLevel, pTex[0], D3DXVECTOR2(1.0f, 0.75f), 0, D3DXVECTOR2(1.0f/2, 0.75f/2), 0xFF3F3F3F );

	//Tworzymy deskê
	new CPaddle( pLevel, pTex[40], pDIDevice );

	//Tworzymy cegie³ki
	for (int y=0; y<BRICK_Y/2; y++)
		for (int x=0; x<BRICK_X; x++)
			if (rand()%4>0)
				new CBrick( pLevel, pTex[52], D3DXVECTOR2( 1.0f/BRICK_X*(0.5f+x), 1.0f/BRICK_Y*(0.5f+y) ) );

	//Tworzymy kulki
	for (int i=0; i<2; i++)
		new CBall( pLevel, pTex[70], D3DXVECTOR2( frand(0.1f,0.9f), frand(0.1f,0.65f) ),
				0.5f * (*D3DXVec2Normalize( &D3DXVECTOR2(), &D3DXVECTOR2( frand(-1,1), frand(-1,1) ) ) ), 
				pTex[50] );

	timerFrameMove.Start();
	fTimeToRender = 0;

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

	list<CMovingSprite*>::iterator	iMovingSprite;
	list<CSprite*>::iterator		iSprite;
	list<CBall*>::iterator			iBall;

	// wykonujemy ruch dla wszystkich obiektów
	for (iMovingSprite = pLevel->listFrameMove.begin(); iMovingSprite != pLevel->listFrameMove.end(); iMovingSprite++)
		(*iMovingSprite)->FrameMove( fElapsedTime );

	// wykonujemy odbicia dla wszystkich obiektów
	for (iSprite = pLevel->listRender.begin(); iSprite != pLevel->listRender.end(); iSprite++)
		(*iSprite)->Collide( &pLevel->listBall );

	// Kasujemy z listy obiektów ruchomych
	iMovingSprite = pLevel->listFrameMove.begin(); 
	while (iMovingSprite != pLevel->listFrameMove.end()) {
		if ((*iMovingSprite)->bDeleteMe)
			iMovingSprite = pLevel->listFrameMove.erase( iMovingSprite );
		else
			iMovingSprite++;
	}

	// Kasujemy z listy kulek
	iBall = pLevel->listBall.begin(); 
	while (iBall != pLevel->listBall.end()) {
		if ((*iBall)->bDeleteMe)
			iBall = pLevel->listBall.erase( iBall );
		else
			iBall++;
	}

	// Kasujemy z listy renderowania
	iSprite = pLevel->listRender.begin();
	while (iSprite != pLevel->listRender.end()) {
		if ((*iSprite)->bDeleteMe) {
			delete (*iSprite);
			iSprite = pLevel->listRender.erase( iSprite );
		}
		else
			iSprite++;
	}

	//GAME OVER !!!!
	if (pLevel->listBall.empty())
		return E_FAIL;

	return S_OK;
}


HRESULT CD3DBallApp::Render()
{
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	numRender++;

	//pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CSprite*>::iterator iSprite;
	for (iSprite = pLevel->listRender.begin(); iSprite != pLevel->listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}

HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );
	return S_OK;
}

HRESULT CD3DBallApp::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );
	return S_OK;
}

HRESULT CD3DBallApp::DeleteDeviceObjects()
{
	delete pLevel;

	for (int i=0; i<256; i++)
		SAFE_RELEASE( pTex[i] );

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

HRESULT CD3DBallApp::LoadTexture( LPCTSTR nameTexture, DWORD numTex )
{
	D3DXCreateTextureFromFileEx( pd3dDevice, nameTexture, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED ,D3DX_DEFAULT,
		D3DX_DEFAULT ,0x00FF00FF, NULL, NULL, &pTex[numTex] );

	return S_OK;
}
