#include "StdAfx.h"
#include "GameEditor.h"
#include "Cursor.h"

#include <fstream>		//TMP

CGameEditor::CGameEditor( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CGameBoard( d3dDevice, DIDevice )
{
	pCursor		= NULL;
	pSprite		= NULL;
	curType = 0;
}


CGameEditor::~CGameEditor()
{
}


HRESULT CGameEditor::InitDeviceObjects()
{
	CGameBoard::InitDeviceObjects();

	LPDIRECT3DTEXTURE8 pTex;
	LoadTexture( "gfx/Cursor_arrow.png", &pTex );
	pCursor = new CCursor( pTex );
	listRender.push_back( pCursor );

	return S_OK;
}

HRESULT CGameEditor::RenderLoop()
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

	for (DWORD i=0; i<dwElements; i++) {
		switch (didod[ i ].dwOfs) {
			case DIMOFS_X:
				pCursor->Move( D3DXVECTOR2( MOUSE_SPEED * (int)didod[ i ].dwData, 0 ) );
				break;
			case DIMOFS_Y:
				pCursor->Move( D3DXVECTOR2( 0, MOUSE_SPEED * (int)didod[ i ].dwData ) );
				break;
            case DIMOFS_BUTTON0:
            case DIMOFS_BUTTON1:
				if (didod[ i ].dwData & 0x80) {
					POINT pos;
					pos.x = (LONG)((pCursor->vPosition.x - BOARD_L) * BRICK_X / BOARD_W);
					pos.y = (LONG)((pCursor->vPosition.y - BRICK_TABLE_T) * BRICK_Y / BRICK_TABLE_H);
					if (pos.x>=0 && pos.x<BRICK_X && pos.y>=0 && pos.y<BRICK_Y) {
						if (pBrickTable[pos.x][pos.y]) {
							curType = pBrickTable[pos.x][pos.y]->dwType;
							if (didod[ i ].dwOfs == DIMOFS_BUTTON0)
								curType = (curType + 1)%BRICK_TYPES;
							RemoveBrick( pos );
						}

						if (didod[ i ].dwOfs == DIMOFS_BUTTON0)
							InsertBrick( curType, pos );
					}
				}
				break;
        }
	
	}

	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	CGameBoard::RenderLoop();

	pSprite->End();
	pd3dDevice->EndScene();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}

//TODO: jeœli funkcja wywo³uje funkcje klasy bazowej zwracaæ wartoœæ

HRESULT CGameEditor::DeleteDeviceObjects()
{
	SaveLevel( "lev/level.lev" );
	
	return CGameBoard::DeleteDeviceObjects();
}
