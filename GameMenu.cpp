#include "stdafx.h"
#include "gamemenu.h"
#include "menuitem.h"
#include "cursor.h"

#include "gameengine.h"
#include "gameeditor.h"

CGameMenu::CGameMenu( LPDIRECT3DDEVICE8 d3dDevice )
	: CD3DScene( d3dDevice )
{
	pSprite		= NULL;
	pCursor		= NULL;

	pPressedMenuItem = NULL;
	pClickedMenuItem = NULL;
}

HRESULT CGameMenu::InitDeviceObjects()
{
	LPDIRECT3DTEXTURE8 pTex;

	CMenuItem* pMenuItem;
	LoadTexture( "gfx/Menu_title.png", &pTex );
	pMenuItem = new CMenuItem( pTex, D3DXVECTOR2( 0.8f, 0.2f ), D3DXVECTOR2( 0.5f, 0.15f ), 0xFFFFFFFF, 0 );
	listRender.push_back( pMenuItem );

	LoadTexture( "gfx/Menu_start.png", &pTex );
	pMenuItem = new CMenuItem( pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.4f ), 0xFFFFCC00, UID_START );
	listRender.push_back( pMenuItem );
	listMenuItem.push_back( pMenuItem );

	LoadTexture( "gfx/Menu_editor.png", &pTex );
	pMenuItem = new CMenuItem( 	pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.5f ), 0xFFFFCC00, UID_EDITOR );
	listRender.push_back( pMenuItem );
	listMenuItem.push_back( pMenuItem );

	LoadTexture( "gfx/Menu_quit.png", &pTex );
	pMenuItem = new CMenuItem( 	pTex, D3DXVECTOR2( 0.4f, 0.1f ), D3DXVECTOR2( 0.5f, 0.6f ), 0xFFFFCC00, UID_QUIT );
	listRender.push_back( pMenuItem );
	listMenuItem.push_back( pMenuItem );

	LoadTexture( "gfx/Cursor_arrow.png", &pTex );
	pCursor = new CCursor( pTex );
	listRender.push_back( pCursor );

	pCurMenuItem = GetPointedMenuItem();
	if (pCurMenuItem) pCurMenuItem->SetHighlighted( true );

	return S_OK;
}


HRESULT CGameMenu::RestoreDeviceObjects()
{
	D3DXCreateSprite( pD3DDevice, &pSprite );

	return S_OK;
}

CMenuItem* CGameMenu::GetPointedMenuItem() const
{
	list<CMenuItem*>::const_iterator iMenuItem;
	for (iMenuItem = listMenuItem.begin(); iMenuItem != listMenuItem.end(); iMenuItem++)
		if (fabs((*iMenuItem)->vPosition.y - pCursor->vPosition.y) < (*iMenuItem)->vSize.y/2 &&
			fabs((*iMenuItem)->vPosition.x - pCursor->vPosition.x) < (*iMenuItem)->vSize.x/2 )
			break;
	return (iMenuItem != listMenuItem.end()) ? *iMenuItem : NULL;
}


HRESULT CGameMenu::ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod )
{
	switch (didod->dwOfs) {
		case DIMOFS_X:
			pCursor->Move( D3DXVECTOR2( (float)(int)didod->dwData, 0 ) );
			break;
		case DIMOFS_Y:
			pCursor->Move( D3DXVECTOR2( 0, (float)(int)didod->dwData ) );
			break;
	}

	CMenuItem* pOldMenuItem;

	switch (didod->dwOfs)
    {
        case DIMOFS_X:
		case DIMOFS_Y:
			pOldMenuItem = pCurMenuItem;
			pCurMenuItem = GetPointedMenuItem();
			if (pPressedMenuItem && pCurMenuItem != pPressedMenuItem)
				pCurMenuItem = NULL;
			if (pCurMenuItem != pOldMenuItem) {
				if (pOldMenuItem)
					pOldMenuItem->SetHighlighted( false );
				if (pCurMenuItem)
					pCurMenuItem->SetHighlighted( true );
			}
            break;
	

        case DIMOFS_BUTTON0:
			if (didod->dwData & 0x80) {		// przycisk nacisniety
					if ( pCurMenuItem ) {
					pPressedMenuItem = pCurMenuItem; 
					pPressedMenuItem->SetPressed( true );
				}
			}
			else {
				if (pPressedMenuItem ) {		// przycisk puszczony
					pPressedMenuItem->SetPressed( false );
					if (pCurMenuItem == pPressedMenuItem)
						pClickedMenuItem = pCurMenuItem;
					pPressedMenuItem = NULL;
				}
			}
			break;
    }

	return S_OK;
}


HRESULT CGameMenu::ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )
{
	return S_OK;
}

HRESULT CGameMenu::FrameMove( float fElapsedTime )
{
	return S_OK;
}


HRESULT CGameMenu::FrameRender()
{
	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(58,110,145), 1.0f, 0 );

	pD3DDevice->BeginScene();
	pSprite->Begin();

	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pD3DDevice->EndScene();
	
	return S_OK;
}

CD3DScene* CGameMenu::GetNextScene()
{
	CD3DScene* pScene = this;
	if (pClickedMenuItem) {
		switch (pClickedMenuItem->GetUID()) {
			case UID_START:
				pScene = new CGameEngine( pD3DDevice );
				break;
			case UID_EDITOR:
				pScene = new CGameEditor( pD3DDevice );
				break;
			case UID_QUIT:
				pScene = NULL;
				break;
		}
		pClickedMenuItem = NULL;
	}

	return pScene;
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


CGameMenu::~CGameMenu()
{
}
