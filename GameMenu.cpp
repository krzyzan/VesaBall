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

HRESULT CGameMenu::AddCursor( LPDIRECT3DTEXTURE8 pTex )
{
	CCursor* pCursor = new CCursor( pTex, pDIDevice, &listMenuItem );
	listRender.push_back( pCursor );
	listFrameMove.push_back( pCursor );

	return S_OK;
}

HRESULT CGameMenu::AddMenuTitle( LPDIRECT3DTEXTURE8 pTex, const D3DXVECTOR2 & Size, 
	const D3DXVECTOR2 & Position, D3DCOLOR Blending )
{
	CSprite* pSprite = new CSprite(	pTex, Size, 0, Position, Blending );
	listRender.push_back( pSprite );

	return S_OK;
}


CGameMenu::~CGameMenu()
{
}
