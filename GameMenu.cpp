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


	return S_OK;
}


HRESULT CGameMenu::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}

CMenuItem* CGameMenu::GetPointedMenuItem()
{
	list<CMenuItem*>::iterator iMenuItem;
	for (iMenuItem = listMenuItem.begin(); iMenuItem != listMenuItem.end(); iMenuItem++)
		if (fabs((*iMenuItem)->vPosition.y - pCursor->vPosition.y) < (*iMenuItem)->vSize.y/2 &&
			fabs((*iMenuItem)->vPosition.x - pCursor->vPosition.x) < (*iMenuItem)->vSize.x/2 )
			break;
	return (iMenuItem != listMenuItem.end()) ? *iMenuItem : NULL;
}


HRESULT CGameMenu::RenderLoop()
{
    DIDEVICEOBJECTDATA didod[ MOUSE_BUFFER_SIZE ];  // Receives buffered data 
    DWORD              dwElements;
    HRESULT            hr;

    if (NULL == pDIDevice) 
        return S_OK;
    
    dwElements = MOUSE_BUFFER_SIZE;
    if (FAILED( hr = pDIDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, &dwElements, 0 ) ) )
        return hr;

	CMenuItem* pCurMenuItem = GetPointedMenuItem();
	if (pCurMenuItem) pCurMenuItem->SetHighlighted( true );

	for (DWORD i=0; i<dwElements; i++) {
		switch (didod[ i ].dwOfs) {
			case DIMOFS_X:
				pCursor->Move( D3DXVECTOR2( MOUSE_SPEED * (int)didod[ i ].dwData, 0 ) );
				break;
			case DIMOFS_Y:
				pCursor->Move( D3DXVECTOR2( 0, MOUSE_SPEED * (int)didod[ i ].dwData ) );
				break;
		}

		switch (didod[ i ].dwOfs)
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
				if (didod[ i ].dwData & 0x80) {		// przycisk nacisniety
						if ( pCurMenuItem ) {
						pPressedMenuItem = pCurMenuItem; 
						pPressedMenuItem->SetPressed( true );
					}
				}
				else {
					if (pPressedMenuItem ) {		// przycisk puszczony
						pPressedMenuItem->SetPressed( false );
						if (pCurMenuItem == pPressedMenuItem) {
							pPressedMenuItem = NULL;		//TODO: TMP
							return pCurMenuItem->GetUID();
						}
						pPressedMenuItem = NULL;
					}
				}
				break;
        }
	}

////////	
	
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


CGameMenu::~CGameMenu()
{
}
