#include "StdAfx.h"
#include "GameEditor.h"
#include "Cursor.h"
#include "Brick.h"

#include <fstream>		//TMP

CGameEditor::CGameEditor( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CGameBoard( d3dDevice, DIDevice )
{
	pCursor		= NULL;
	pSprite		= NULL;
	curType		= 0;
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
					if ( !pBrickArray->Contains( pCursor->vPosition ) ) 
						break;

					POINT pos = pBrickArray->VectorToArrayCoords( pCursor->vPosition );
					CBrick* iNewBrick = pBrickArray->GetBrickAt( pos );
					if ( iNewBrick->pTypeDesc ) {
						curType = LONG(CBrick::s_TypeDesc - iNewBrick->pTypeDesc);
						if (didod[ i ].dwOfs == DIMOFS_BUTTON0)
							curType = (curType + 1)%CBrick::TYPE_MAX;
						iNewBrick->pTypeDesc = NULL;
					}

					if (didod[ i ].dwOfs == DIMOFS_BUTTON0) {
						pBrickArray->CreateBrick( pos, curType );
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
	pCursor->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}


HRESULT CGameEditor::DeleteDeviceObjects()
{
	pBrickArray->Save( "lev/level.lev" );
	delete pCursor;

	return CGameBoard::DeleteDeviceObjects();
}
