#include "stdafx.h"
#include "cursor.h"

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

void CCursor::FrameMove( FLOAT fElapsedTime )
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) return;
    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	D3DXVECTOR2 vMovement = D3DXVECTOR2((FLOAT)dims2.lX, (FLOAT)dims2.lY);

	if (vMovement == D3DXVECTOR2(0,0)) 
		return;
	
	vPosition += vMovement * 0.002f;

	//if (dims2.rgbButtons[0]) exit(0);

	if (vPosition.x < 0.00f) vPosition.x = 0.00f;
	if (vPosition.x > 1.00f) vPosition.x = 1.00f;
	if (vPosition.y < 0.00f) vPosition.y = 0.00f;
	if (vPosition.y > 0.75f) vPosition.y = 0.75f;

	list<CMenuItem*>::iterator iMenuItem;
	for (iMenuItem = pListMenuItem->begin(); iMenuItem != pListMenuItem->end(); iMenuItem++) {
		CMenuItem* pMenuItem = (*iMenuItem);

		if (fabs(pMenuItem->vPosition.y - vPosition.y) < pMenuItem->vSize.y/2 &&
			fabs(pMenuItem->vPosition.x - vPosition.x) < pMenuItem->vSize.x/2 )
		{
			//pMenuItem->dwBlending = 0x7FFFFFFF;
			pMenuItem->dwBlending |= ~0xFFFFCC00;
		}
		else
			//pMenuItem->dwBlending = 0xFFFFFFFF;
			pMenuItem->dwBlending &=  0xFFFFCC00;
	}
}

void CCursor::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2) * RES_X;
	Position = D3DXVECTOR2( floor( Position.x ), floor( Position.y ) );
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &Position, dwBlending );
}
