#include "StdAfx.h"
#include "GameEditor.h"
#include "Cursor.h"
#include "Brick.h"

#include <fstream>		//TMP

CGameEditor::CGameEditor( LPDIRECT3DDEVICE8 d3dDevice )
	: CGameBoard( d3dDevice )
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


HRESULT CGameEditor::ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod )
{
	switch (didod->dwOfs) {
		case DIMOFS_X:
			pCursor->Move( D3DXVECTOR2( (float)(int)didod->dwData, 0 ) );
			break;
		case DIMOFS_Y:
			pCursor->Move( D3DXVECTOR2( 0, (float)(int)didod->dwData ) );
			break;
        case DIMOFS_BUTTON0:
        case DIMOFS_BUTTON1:
			if (didod->dwData & 0x80) {
				if ( pBrickArray->Contains( pCursor->vPosition ) ) {
					POINT pos = pBrickArray->GetArrayCoords( pCursor->vPosition );
					CBrick* pBrick = pBrickArray->GetBrick( pos );
					if (pBrick) {
						curType = pBrick->GetType();
						if (didod->dwOfs == DIMOFS_BUTTON0)
							curType = (curType + 1)%BRICK_TYPE_MAX;
						pBrickArray->RemoveBrick( pos );
					}

					if (didod->dwOfs == DIMOFS_BUTTON0) {
						pBrickArray->InsertBrick( curType, pos );
					}
				}
			}
			break;
    }

	return S_OK;
}


HRESULT CGameEditor::ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )
{
	// TODO: TMP
	//if (didod[ i ].dwOfs == DIK_SPACE && (didod[ i ].dwData & 0x80) )
	//	exit(0);

	return S_OK;
}


HRESULT CGameEditor::FrameMove( float fElapsedTime )
{
	return S_OK;
}


HRESULT CGameEditor::FrameRender()
{
	// renderujemy
	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	pD3DDevice->BeginScene();
	pSprite->Begin();

	CGameBoard::FrameRender();

	pCursor->Render( pSprite );

	pSprite->End();
	pD3DDevice->EndScene();

	return S_OK;
}


CD3DAppScene* CGameEditor::GetNextScene()
{
	return this;
}


//TODO: jeœli funkcja wywo³uje funkcje klasy bazowej zwracaæ wartoœæ

HRESULT CGameEditor::DeleteDeviceObjects()
{
	pBrickArray->Save( "lev/level.lev" );
	delete pCursor;

	return CGameBoard::DeleteDeviceObjects();
}
