// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v0.15
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DBallApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD3DBallApp::CD3DBallApp()
: CD3DApp( RES_X, RES_Y )
{	
	srand( (INT)Timer.GetTime() );
	numFrameMove	= 0;
	numRender		= 0;
	pSprite			= NULL;
	ZeroMemory( pTex, sizeof(pTex) );
}

CD3DBallApp::~CD3DBallApp()
{
}


HRESULT CD3DBallApp::InitDeviceObjects()
{
	LoadTexture( "gfx/Deckzatruta.png",		0 );
	LoadTexture( "gfx/Decknormalna.png",	1 );
	LoadTexture( "gfx/Tree2.jpg",			2 );
	LoadTexture( "gfx/kulkaa.png",			3 );
	LoadTexture( "gfx/kulkab.png",			4 );
	LoadTexture( "gfx/kulkac.png",			5 );
	LoadTexture( "gfx/kulkad.png",			6 );
	LoadTexture( "gfx/kulkae.png",			7 );
	LoadTexture( "gfx/kulkaf.png",			8 );
	LoadTexture( "gfx/kulkag.png",			9 );
	LoadTexture( "gfx/kulkah.png",			10 );
	LoadTexture( "gfx/kulkai.png",			11 );
	LoadTexture( "gfx/kulkaj.png",			12 );
	LoadTexture( "gfx/kulkak.png",			13 );
	LoadTexture( "gfx/kulkal.png",			14 );
	LoadTexture( "gfx/kulkam.png",			15 );
	LoadTexture( "gfx/kulkan.png",			16 );
	LoadTexture( "gfx/kulkao.png",			17 );
	LoadTexture( "gfx/Cellblue.png",		18 );
	LoadTexture( "gfx/Cellred.png",			19 );
	LoadTexture( "gfx/Cellgren.png",		20 );
	LoadTexture( "gfx/Cellmetala.png",		21 );
	LoadTexture( "gfx/Cellmetalb.png",		22 );
	LoadTexture( "gfx/Cellfoliaa.png",		23 );
	LoadTexture( "gfx/Cellfoliab.png",		24 );
	LoadTexture( "gfx/Cellfoliac.png",		25 );
	LoadTexture( "gfx/Cellfoliad.png",		26 );
	LoadTexture( "gfx/Cellwybucha.png",		27 );
	LoadTexture( "gfx/Cellwybuchb.png",		28 );
	LoadTexture( "gfx/Cellcool.png",		29 );
	LoadTexture( "gfx/Cellbluegren.png",	30 );
	LoadTexture( "gfx/SparkEffect.png",		50 );

	pBackground = new CSprite( pTex[2], D3DXVECTOR2(1.0f, 0.75f), 0, D3DXVECTOR2(1.0f/2, 0.75f/2), 0xFF7F7F7F );

	//Tworzymy deskê
	CDeck* pDeck;
	pDeck = new CDeck( pTex[1], pDIDevice );
	listRender.push_back( pDeck );

	//Tworzymy cegie³ki
	CBrick* pBrick;
	for (int i=1; i<128; i++) {
		pBrick = new CBrick( pTex[22], D3DXVECTOR2( 1.0f/BRICK_X*(0.5f+(rand()%BRICK_X)), 1.0f/BRICK_Y*(0.5f+(rand()%(BRICK_Y/2))) ) );
		listRender.push_back( pBrick );
	}

	//Tworzymy kulki
	CBall* pBall;
	for (int i=0; i<4; i++) {
		pBall = new CBall( pTex[6], 
			D3DXVECTOR2( FLOAT((rand()%1000-200)+100)/1000, FLOAT((rand()%750-200)+100)/1000 ),
			D3DXVECTOR2( 1.0f*(rand()%2000-1000), 1.0f*(rand()%2000-1000))/2000,
			&listRender, pTex[50] );
		listRender.push_back( pBall );
		listBall.push_back( pBall );
	}

	timerFrameMove.Start();
	fTimeToRender = 0;

	return S_OK;
}


HRESULT CD3DBallApp::FrameMove()
{
	FLOAT fElapsedTime = timerFrameMove.GetElapsedTime();
	fTimeToRender -= fElapsedTime;

	//TMP: u mnie czasem zawiesza siê timer na 2 sek.
	if ( fElapsedTime > 0.1 ) return S_OK;	
	numFrameMove++;

	list<CSprite*>::iterator iSprite;

	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->FrameMove( fElapsedTime );

	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Collide( &listBall );


	// Kasujemy obiekty
	list<CBall*>::iterator iBall = listBall.begin(); 
	while (iBall != listBall.end()) {
		if ((*iBall)->bDeleteMe)
			iBall = listBall.erase( iBall );
		else
			iBall++;
	}

	iSprite = listRender.begin();
	while (iSprite != listRender.end()) {
		if ((*iSprite)->bDeleteMe) {
			delete (*iSprite);
			iSprite = listRender.erase( iSprite );
		}
		else
			iSprite++;
	}

	// GAME OVER !!!!
	//if (listBall.empty())
	//	return E_FAIL;

	return S_OK;
}


HRESULT CD3DBallApp::Render()
{
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/100;
	numRender++;
	//pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	pd3dDevice->BeginScene();

	pSprite->Begin();

	// t³o
	pBackground->Render( pSprite );

	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
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
	for (int i=0; i<256; i++)
		SAFE_RELEASE( pTex[i] );

	char str[100];
	_itoa((INT)(numFrameMove/numRender),str,10);
	MessageBox( hWnd, str, "numFrameMove/numRender", MB_OK );

	return S_OK;
}

HRESULT CD3DBallApp::LoadTexture( LPCTSTR nameTexture, DWORD numTex )
{
	D3DXCreateTextureFromFileEx( pd3dDevice, nameTexture, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED ,D3DX_DEFAULT,
		D3DX_DEFAULT ,0x00FF00FF, NULL, NULL, &pTex[numTex] );

	return S_OK;
}
