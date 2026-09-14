#include "StdAfx.h"
#include "GameEditor.h"
#include "GameMenu.h"

#include "Counter.h"
#include "Cursor.h"
#include "Brick.h"


CGameEditor::CGameEditor()
{
	pCursor			= NULL;
	curType			= 1;
	pLevelCounter	= 0; 

	bLMB = false;
	bRMB = false;
}


CGameEditor::~CGameEditor()
{
}


HRESULT CGameEditor::OnInitDevice()
{
	CGameBoard::OnInitDevice();

	LPDIRECT3DTEXTURE8 pTex;
	LoadTexture( "gfx/Cursor_arrow.png", &pTex );
	pCursor = new CCursor( pTex );

	// tworzymy licznik poziomu
	LoadTexture( "gfx/Digits.png",&CCounter::spTexture );
	POINT DigitPixels = {64,92};
	pLevelCounter = new CCounter( 0,D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), DigitPixels, 6 );

	// tworzymy legendê cegie³ek
	POINT arraySize = {10,4};
	pBrickToolkit = new CBrickArray( arraySize, D3DXVECTOR2( BOARD_L+BOARD_W/2, BOARD_B-BOARD_W/10 ), D3DXVECTOR2( BOARD_W/2, BOARD_W/10 ) );
	
	POINT pos;
	BYTE idType = 0;
	for (pos.y=0; pos.y<arraySize.y; pos.y++)
		for (pos.x=0; pos.x<arraySize.x; pos.x++) {
			idType++;
			if (idType >= BRICK_TYPE_MAX)
				return S_OK;
			pBrickToolkit->InsertBrick( idType, pos);
		}

	return S_OK;
}


HRESULT CGameEditor::OnMouseEvent( LPDIDEVICEOBJECTDATA didod )
{
	switch (didod->dwOfs) {
		case DIMOFS_X:
			pCursor->Move( D3DXVECTOR2( (float)(int)didod->dwData, 0 ) );
			break;

		case DIMOFS_Y:
			pCursor->Move( D3DXVECTOR2( 0, (float)(int)didod->dwData ) );
			break;

		case DIMOFS_BUTTON0:
			if (didod->dwData & 0x80) {
				POINT pos = pBrickToolkit->GetArrayCoordsAt( pCursor->vPosition );
				if ( pBrickToolkit->IsValid( pos ) ) {
					CBrick* pBrick = pBrickToolkit->GetBrick( pos );
					if (pBrick)
						curType = pBrick->GetType();
				}
			}
			bLMB = ((didod->dwData & 0x80) != 0);
			break;

		case DIMOFS_BUTTON1:
			bRMB = ((didod->dwData & 0x80) != 0);
			break;
    }

	return S_OK;
}

HRESULT CGameEditor::OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod )
{
	if ( didod->dwData & 0x80 )
		switch (didod->dwOfs) {
			case DIK_RIGHT:
				if (dwLevelNum < NUM_LEVELS-1) {
					dwLevelNum++;
					pLevelCounter->lValue = dwLevelNum;
					pBrickArray->Load( dwLevelNum );
				}
				return S_OK;

			case DIK_LEFT:
				if (dwLevelNum > 0) {
					dwLevelNum--;
					pLevelCounter->lValue = dwLevelNum;
					pBrickArray->Load( dwLevelNum );
				}
				return S_OK;

			case DIK_L:
				pBrickArray->Load( dwLevelNum );
				return S_OK;

			case DIK_S:
				pBrickArray->Save( dwLevelNum );
				return S_OK;

			case DIK_C:
				pBrickArray->Clear();
				return S_OK;
		}

	return CGameBoard::OnKeyboardEvent( didod );
}


HRESULT CGameEditor::FrameMove( float fElapsedTime )
{
	pLevelCounter->Update( fElapsedTime );

	if ( bLMB || bRMB ) {
		POINT pos = pBrickArray->GetArrayCoordsAt( pCursor->vPosition );
		if ( pBrickArray->IsValid( pos ) ) {
			pBrickArray->RemoveBrick( pos );
            if (bLMB)
				pBrickArray->InsertBrick( curType, pos );
		}
	}

	return S_OK;
}


HRESULT CGameEditor::FrameRender()
{
	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	CGameBoard::FrameRender();

	pSprite->Begin();
	pBrickToolkit->Render( pSprite );
	pLevelCounter->Render( pSprite );
	pCursor->Render( pSprite );
	pSprite->End();

	return S_OK;
}


HRESULT CGameEditor::OnDeleteDevice()
{
	SAFE_DELETE( pCursor );
	SAFE_DELETE( pLevelCounter );

	return CGameBoard::OnDeleteDevice();
}
