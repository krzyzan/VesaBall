// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v0.30
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
	
	LPDIRECT3DTEXTURE8 pTex;
	LPDIRECT3DTEXTURE8 pSparkTex;

	//Tworzymy deskê
	pLevel->LoadTexture( "gfx/Paddle.png", &pTex );
	pLevel->AddPaddle( pTex );

	//Tworzymy cegie³ki
	pLevel->LoadTexture( "gfx/Brick5.png", &pTex );
	int x, y;
	for (y=0; y<BRICK_Y/2; y++)
		for (x=0; x<BRICK_X; x++)
			if (rand()%4>0)
				pLevel->AddBrick( pTex, D3DXVECTOR2( (0.5f+x)/BRICK_X, (0.5f+y)/BRICK_Y ), D3DXVECTOR2(1.0f/BRICK_X, 1.0f/BRICK_Y) );

	//Tworzymy kulki
	pLevel->LoadTexture( "gfx/Ball_alu.png", &pTex );
	pLevel->LoadTexture( "gfx/SparkEffect.png", &pSparkTex );
	pLevel->AddBall( pTex, D3DXVECTOR2( frand(0.1f,0.9f), frand(0.1f,0.65f) ),
			0.5f * (*D3DXVec2Normalize( &D3DXVECTOR2(), &D3DXVECTOR2( frand(-1,1), frand(-1,1) ) ) ), pSparkTex );

	return S_OK;
}

HRESULT CD3DBallApp::EndScene()
{
	delete pScene;
	pScene = NULL;
    
	//TODO: Kasowanie tekstur

	return S_OK;
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
			delete pScene;
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
	delete pScene;
	return S_OK;
}

HRESULT	CD3DBallApp::FinalCleanup()
{
	return S_OK;
}
