#include "stdafx.h"
#include "gamemenu.h"
#include "menuitem.h"
#include "cursor.h"


CGameMenu::CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{
	pSprite		= NULL;
	pCursor		= NULL;

	pOldMenuItem = NULL;
	pPressedMenuItem = NULL;
	bOldButtonState = false;
}

HRESULT CGameMenu::InitDeviceObjects()
{
	LPDIRECT3DTEXTURE8 pTex;

	LoadTexture( "gfx/Menu_title.png", &pTex );
    AddMenuTitle( pTex, D3DXVECTOR2( 0.8f, 0.2f ), D3DXVECTOR2( 0.5f, 0.15f ),	0xFFFFFFFF );

	LoadTexture( "gfx/Menu_start.png", &pTex );
	AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.4f ), 0xFFFFCC00, UID_START );

	LoadTexture( "gfx/Menu_editor.png", &pTex );
	AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.5f ), 0xFFFFCC00, UID_EDITOR );

	LoadTexture( "gfx/Menu_quit.png", &pTex );
	AddMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.6f ), 0xFFFFCC00, UID_QUIT );

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

////////

	DIMOUSESTATE2 dims2;
	ZeroMemory( &dims2, sizeof(dims2) );
	pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

	pCursor->MouseMove( &dims2 );

	CMenuItem* pCurMenuItem; 
	if ( pPressedMenuItem ) {
		if (fabs(pPressedMenuItem->vPosition.y - pCursor->vPosition.y) < pPressedMenuItem->vSize.y/2 &&
				fabs(pPressedMenuItem->vPosition.x - pCursor->vPosition.x) < pPressedMenuItem->vSize.x/2 )
			pCurMenuItem = pPressedMenuItem;
		else
			pCurMenuItem = NULL;
	}
	else {
		list<CMenuItem*>::iterator iMenuItem;
		for (iMenuItem = listMenuItem.begin(); iMenuItem != listMenuItem.end(); iMenuItem++)
			if (fabs((*iMenuItem)->vPosition.y - pCursor->vPosition.y) < (*iMenuItem)->vSize.y/2 &&
					fabs((*iMenuItem)->vPosition.x - pCursor->vPosition.x) < (*iMenuItem)->vSize.x/2 )
				break;
		pCurMenuItem = (iMenuItem != listMenuItem.end()) ? *iMenuItem : NULL;
	}

	BOOL bCurButtonState = dims2.rgbButtons[0];

	if (pCurMenuItem != pOldMenuItem) {
		if (pOldMenuItem)
			pOldMenuItem->SetHighlighted( false );
		if (pCurMenuItem)
			pCurMenuItem->SetHighlighted( true );
	}

	// przycisk wcisniety
	if (pCurMenuItem && !bOldButtonState &&  bCurButtonState ) {
		pPressedMenuItem = pCurMenuItem; 
		pPressedMenuItem->SetPressed( true );
	}

	// przycisk puszczony
	if (pPressedMenuItem && bOldButtonState && !bCurButtonState ) {
		pPressedMenuItem->SetPressed( false );
		if (pCurMenuItem == pPressedMenuItem) {
			pPressedMenuItem = NULL;
			return pCurMenuItem->GetUID();
		}
		pPressedMenuItem = NULL;
	}

	pOldMenuItem = pCurMenuItem;
	bOldButtonState = bCurButtonState;

////////	
	
	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(58,110,145), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CSprite*>::iterator iSprite;
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
	list<CSprite*>::iterator iSprite = listRender.begin();
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
	pCursor = new CCursor( pTex );
	listRender.push_back( pCursor );

	return S_OK;
}

CGameMenu::~CGameMenu()
{
}
