#include "stdafx.h"
#include "cursor.h"

#include "menuitem.h"

CCursor::CCursor( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice, list<CMenuItem*>* ListMenuItem )
		: CMovingSprite( Texture, D3DXVECTOR2(1.0f/16, 1.0f/16), 0, 
	D3DXVECTOR2(1.0f/2, 0.75f/2), D3DXVECTOR2(0, 0), 
	D3DXVECTOR2(0, 0), 0xFFFFFFFF )
{
	pListMenuItem = ListMenuItem;
	pDIDevice = DIDevice;
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

	D3DXVECTOR2 vMovement = D3DXVECTOR2((FLOAT)dims2.lX, (FLOAT)dims2.lY);
	//if (vMovement == D3DXVECTOR2(0,0)) 
	//	return;
	vPosition += vMovement * 0.002f;

	D3DXVec2Maximize( &vPosition, &vPosition, &D3DXVECTOR2(0.00f,0.00f ) );
	D3DXVec2Minimize( &vPosition, &vPosition, &D3DXVECTOR2(1.00f,0.75f ) );

	//TODO: Przegl¹dnaæ
	list<CMenuItem*>::iterator iMenuItem;
	for (iMenuItem = pListMenuItem->begin(); iMenuItem != pListMenuItem->end(); iMenuItem++) {
		CMenuItem* pMenuItem = (*iMenuItem);

		if (fabs(pMenuItem->vPosition.y - vPosition.y) < pMenuItem->vSize.y/2 &&
			fabs(pMenuItem->vPosition.x - vPosition.x) < pMenuItem->vSize.x/2 )
		{
			pMenuItem->dwBlending |= ~0xFFFFCC00;

			if (dims2.rgbButtons[0]) {
				pMenuItem->Press();
			}
			else
				if (pMenuItem->bPressed)
					return pMenuItem->dwUID;

		}
		else
			pMenuItem->dwBlending &=  0xFFFFCC00;
	}
	return S_OK;
}

void CCursor::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2) * RES_X;
	Position = D3DXVECTOR2( floor( Position.x ), floor( Position.y ) );
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &Position, dwBlending );
}
