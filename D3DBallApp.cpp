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
	CreateMenu();
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
		case MENU_START:
			pScene->InvalidateDeviceObjects();
			pScene->DeleteDeviceObjects();
			SAFE_DELETE( pScene );
			CreateLevel();
			pScene->InitDeviceObjects();
			pScene->RestoreDeviceObjects();
			break;
		case MENU_QUIT:
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

HRESULT CD3DBallApp::CreateMenu()
{
	CGameMenu* pGameMenu = new CGameMenu( pd3dDevice, pDIDevice );
	pScene = pGameMenu;

	LPDIRECT3DTEXTURE8 pTex;

	pGameMenu->LoadTexture( "gfx/Menu_title.png", &pTex );
    pGameMenu->AddMenuTitle( pTex, D3DXVECTOR2( 0.8f, 0.2f ), D3DXVECTOR2( 0.5f, 0.15f ),	0xFFFFFFFF );

	pGameMenu->LoadTexture( "gfx/Menu_start.png", &pTex );
	pGameMenu->AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.4f ), 0xFFFFCC00, MENU_START );

	pGameMenu->LoadTexture( "gfx/Menu_editor.png", &pTex );
	pGameMenu->AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.5f ), 0xFFFFCC00, MENU_EDITOR );

	pGameMenu->LoadTexture( "gfx/Menu_quit.png", &pTex );
	pGameMenu->AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.6f ), 0xFFFFCC00, MENU_QUIT );

	pGameMenu->LoadTexture( "gfx/Cursor_arrow.png", &pTex );
	pGameMenu->AddCursor( pTex );

	return S_OK;
}

HRESULT CD3DBallApp::CreateLevel()
{
	CLevel* pLevel = new CLevel( hWnd, pd3dDevice, pDIDevice );
	pScene = pLevel;
	
	LPDIRECT3DTEXTURE8 pPaddleTex;
	LPDIRECT3DTEXTURE8 pBallTex;
	LPDIRECT3DTEXTURE8 pSparkTex;
	LPDIRECT3DTEXTURE8 pBrickTex;
	LPDIRECT3DTEXTURE8 pLightningTex;

	//Tworzymy deskê z kulka
	pLevel->LoadTexture( "gfx/Paddle.png", &pPaddleTex );
	pLevel->LoadTexture( "gfx/Ball_alu.png", &pBallTex );
	pLevel->LoadTexture( "gfx/SparkEffect.png", &pSparkTex );
	pLevel->LoadTexture( "gfx/Lightning.png", &pLightningTex );

	//TODO: gdy dodajemy kulke po desce b³yskawica pojawia sie w z³ym mejscu
	pLevel->AddPaddle( pPaddleTex, pLightningTex, pBallTex, pSparkTex );

	//Tworzymy cegie³ki
	pLevel->LoadTexture( "gfx/Brick5.png", &pBrickTex );

	float x, y;
	for (y=BOARD_T+BOARD_H/2/BRICK_Y; y<BOARD_T+BOARD_H*0.75; y+=BOARD_H/BRICK_Y)
		for (x=BOARD_L+BOARD_W/2/BRICK_X; x<BOARD_R; x+=BOARD_W/BRICK_X)
			if (rand()%4>0)
				pLevel->AddBrick( pBrickTex, D3DXVECTOR2( x, y ), 
					D3DXVECTOR2( BOARD_W/BRICK_X, BOARD_H/BRICK_Y) );


	return S_OK;
}

