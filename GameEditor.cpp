#include "StdAfx.h"
#include "GameEditor.h"
#include "GameMenu.h"

#include "Counter.h"
#include "Cursor.h"
#include "Brick.h"


CGameEditor::CGameEditor()
{
	pCursor			= NULL;
	curType			= 0;
	pLevelCounter	= 0; 
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

	// tworzymy licznik poziomu
	LoadTexture( "gfx/Digits.png",&CCounter::spTexture );
	POINT DigitPixels = {64,92};
	pLevelCounter = new CCounter( 0,D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), DigitPixels, 6 );

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
				if ( !pBrickArray->Contains( pCursor->vPosition ) ) 
					break;

				POINT pos = pBrickArray->GetArrayCoordsAt( pCursor->vPosition );
				CBrick* pBrick = pBrickArray->GetBrick( pos );
				if (pBrick) {
					curType = pBrick->GetType();
					if (didod->dwOfs == DIMOFS_BUTTON0)
						curType = (curType + 1)%BRICK_TYPE_MAX;
					pBrickArray->RemoveBrick( pos );
				}

				if (didod->dwOfs == DIMOFS_BUTTON0) {
					pBrickArray->InsertBrick( curType, pos );
					pBrickArray->GetBrick( pos )->Zap();  // Niewidzialne cegie³ki musz¹ byc widoczne w edytorze
				}
			}
			break;
    }

	return S_OK;
}

HRESULT CGameEditor::ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )
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

	return CGameBoard::ProcessKeybrdEvent( didod );
}


HRESULT CGameEditor::FrameMove( float fElapsedTime )
{
	pLevelCounter->Update( fElapsedTime );
	return S_OK;
}


HRESULT CGameEditor::FrameRender()
{
	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	CGameBoard::FrameRender();

	pSprite->Begin();
	pLevelCounter->Render( pSprite );
	pCursor->Render( pSprite );
	pSprite->End();

	return S_OK;
}


HRESULT CGameEditor::DeleteDeviceObjects()
{
	SAFE_DELETE( pCursor );
	SAFE_DELETE( pLevelCounter );

	return CGameBoard::DeleteDeviceObjects();
}
