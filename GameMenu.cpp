#include "stdafx.h"
#include "gamemenu.h"
#include "menuitem.h"
#include "cursor.h"


CGameMenu::CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{
	pSprite		= NULL;
}

HRESULT CGameMenu::InitDeviceObjects()
{
	LPDIRECT3DTEXTURE8 pTex;

	LoadTexture( "gfx/Menu_title.png", &pTex );
    AddMenuTitle( pTex, D3DXVECTOR2( 0.8f, 0.2f ), D3DXVECTOR2( 0.5f, 0.15f ),	0xFFFFFFFF );

	LoadTexture( "gfx/Menu_start.png", &pTex );
	AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.4f ), 0xFFFFCC00, START );

	LoadTexture( "gfx/Menu_editor.png", &pTex );
	AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.5f ), 0xFFFFCC00, EDITOR );

	LoadTexture( "gfx/Menu_quit.png", &pTex );
	AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.6f ), 0xFFFFCC00, QUIT );

	LoadTexture( "gfx/Cursor_arrow.png", &pTex );
	AddCursor( pTex );

	return S_OK;
}


HRESULT CGameMenu::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CGameMenu::RenderLoop()
{
	// TODO: wywaliæ listê CMovingSprite* zast¹piæ CCursor*

	HRESULT hr;

	// wykonujemy ruch dla wszystkich obiektów
	list<CMovingSprite*>::iterator	iMovingSprite;
	for (iMovingSprite = listFrameMove.begin(); iMovingSprite != listFrameMove.end(); iMovingSprite++) {
        hr = (*iMovingSprite)->FrameMove( 0 );
		if (hr != S_OK)
			return hr;
	}

	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(58,110,145), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CObject*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

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
	list<CObject*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}


HRESULT CGameMenu::AddMenuItem( LPDIRECT3DTEXTURE8 pTex, const D3DXVECTOR2 & Size, 
	const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID )
{
	CMenuItem* pMenuItem = new CMenuItem( pTex, Size, Position, Blending, UID );
	listRender.push_back( pMenuItem );
	listMenuItem.push_back( pMenuItem );

	return S_OK;
}


HRESULT CGameMenu::AddMenuTitle( LPDIRECT3DTEXTURE8 pTex, const D3DXVECTOR2 & Size, 
	const D3DXVECTOR2 & Position, D3DCOLOR Blending )
{
	CMenuItem* pMenuItem = new CMenuItem( pTex, Size, Position, Blending, 0 );
	listRender.push_back( pMenuItem );

	return S_OK;
}


HRESULT CGameMenu::AddCursor( LPDIRECT3DTEXTURE8 pTex )
{
	CCursor* pCursor = new CCursor( pTex, pDIDevice, &listMenuItem );
	listRender.push_back( pCursor );
	listFrameMove.push_back( pCursor );

	return S_OK;
}

CGameMenu::~CGameMenu()
{
}
