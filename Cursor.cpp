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
	pPressedMenuItem = NULL;
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

	CMenuItem* pCurMenuItem; 
	
	if ( pPressedMenuItem ) {
		if (fabs(pPressedMenuItem->vPosition.y - vPosition.y) < pPressedMenuItem->vSize.y/2 &&
				fabs(pPressedMenuItem->vPosition.x - vPosition.x) < pPressedMenuItem->vSize.x/2 )
			pCurMenuItem = pPressedMenuItem;
		else {
			pCurMenuItem = NULL;
		}
	}
	else {
		list<CMenuItem*>::iterator iMenuItem;
		for (iMenuItem = pListMenuItem->begin(); iMenuItem != pListMenuItem->end(); iMenuItem++)
			if (fabs((*iMenuItem)->vPosition.y - vPosition.y) < (*iMenuItem)->vSize.y/2 &&
				fabs((*iMenuItem)->vPosition.x - vPosition.x) < (*iMenuItem)->vSize.x/2 )
					break;
		pCurMenuItem = (iMenuItem != pListMenuItem->end()) ? *iMenuItem : NULL;
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
		if (pCurMenuItem == pPressedMenuItem)
			return pCurMenuItem->GetUID();
		pPressedMenuItem = NULL;
	}

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
