#include "stdafx.h"
#include "cursor.h"

#include "menuitem.h"

CCursor::CCursor( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice, list<CMenuItem*>* ListMenuItem )
		: CMovingSprite( Texture, D3DXVECTOR2(1.0f/16, 1.0f/16), 0, 
	D3DXVECTOR2( BOARD_W/2, BOARD_H/2 ), D3DXVECTOR2(0, 0), 
	D3DXVECTOR2(0, 0), 0xFFFFFFFF )
{
	pListMenuItem = ListMenuItem;
	pDIDevice = DIDevice;
	pOldMenuItem = NULL;
	bOldButtonState = false;
}


CCursor::~CCursor()
{
}

HRESULT CCursor::FrameMove( FLOAT fElapsedTime )
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) 
		return S_OK;

    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	vPosition += D3DXVECTOR2((FLOAT)dims2.lX, (FLOAT)dims2.lY) * 0.002f;

	D3DXVec2Maximize( &vPosition, &vPosition, &D3DXVECTOR2( 0.00f, 0.00f ) );
	D3DXVec2Minimize( &vPosition, &vPosition, &D3DXVECTOR2( 1.00f, 0.75f ) );

	//TODO: Przegl¹dnaæ


	list<CMenuItem*>::iterator iMenuItem;
	for (iMenuItem = pListMenuItem->begin(); iMenuItem != pListMenuItem->end(); iMenuItem++)
		if (fabs((*iMenuItem)->vPosition.y - vPosition.y) < (*iMenuItem)->vSize.y/2 &&
			fabs((*iMenuItem)->vPosition.x - vPosition.x) < (*iMenuItem)->vSize.x/2 )
				break;

	CMenuItem* pCurMenuItem = (iMenuItem != pListMenuItem->end()) ? *iMenuItem : NULL;
	BOOL bCurButtonState = dims2.rgbButtons[0];

	BOOL bButtonPressed = ( !bOldButtonState &&  bCurButtonState );
	BOOL bButtonReleased = ( bOldButtonState && !bCurButtonState );

	if (pCurMenuItem != pOldMenuItem) {
		if (pOldMenuItem)
			pOldMenuItem->dwBlending &= 0xFFFFCC00;
		if (pCurMenuItem)
			pCurMenuItem->dwBlending |= ~0xFFFFCC00;
	}

	if (pOldMenuItem && bButtonPressed ) {
		pOldMenuItem->Press();
		pPressedMenuItem = pPressedMenuItem;
	}

	if (pOldMenuItem && bButtonReleased ) {
		pOldMenuItem->Release();
	}

	/*
	if (pOldMenuItem && pCurMenuItem != pOldMenuItem && dims2.rgbButtons[0]) {
		pOldMenuItem->Release();
	}
	*/

	//return pOldMenuItem->dwUID;
	pOldMenuItem = pCurMenuItem;
	bOldButtonState = bCurButtonState;

	return S_OK;
}

void CCursor::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2) * RES_X;
	Position = D3DXVECTOR2( floor( Position.x ), floor( Position.y ) );
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &Position, dwBlending );
}
