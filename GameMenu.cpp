#include "stdafx.h"
#include "gamemenu.h"
#include "menuitem.h"

CGameMenu::CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{
	ZeroMemory( pTex, sizeof(pTex) );
	pSprite		= NULL;
}

HRESULT CGameMenu::InitDeviceObjects()
{
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Cursor_arrow.png",	&pTex[0] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Menu_title.png",	&pTex[1] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Menu_start.png",	&pTex[2] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Menu_editor.png",	&pTex[3] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Menu_quit.png",		&pTex[4] );

	// tytu³ menu
	CSprite* pSprite = new CSprite( 
		pTex[1], D3DXVECTOR2( 0.8f, 0.2f ), 0,
		D3DXVECTOR2( 0.5f, 0.15f ), 0xFFFFFFFF );
	listRender.push_back( pSprite );

	// pozycje menu
	for (int i=2; i<5; i++) {
		CMenuItem* pMenuItem = new CMenuItem( pTex[i], 
			D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, i*0.1f+0.2f ), 
			0xFFFFFFFF );
		listRender.push_back( pMenuItem );
		listMenuItem.push_back( pMenuItem );
	}

	// kursor
	CCursor* pCursor = new CCursor( pTex[0], pDIDevice, &listMenuItem );
	listRender.push_back( pCursor );
	listFrameMove.push_back( pCursor );

	return S_OK;
}

HRESULT CGameMenu::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CGameMenu::FrameMove( FLOAT fElapsedTime )
{
	list<CMovingSprite*>::iterator	iMovingSprite;

	// wykonujemy ruch dla wszystkich obiektów
	for (iMovingSprite = listFrameMove.begin(); iMovingSprite != listFrameMove.end(); iMovingSprite++)
		(*iMovingSprite)->FrameMove( fElapsedTime );

	return S_OK;
}


HRESULT CGameMenu::Render()
{
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(58,110,145), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	return S_OK;
}


HRESULT CGameMenu::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );
	
	return S_OK;
}


HRESULT CGameMenu::DeleteDeviceObjects()
{
	// Kasujemy z listy renderowania
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	for (int i=0; i<256; i++)
		SAFE_RELEASE( pTex[i] );

	return S_OK;
}


CGameMenu::~CGameMenu()
{
}
