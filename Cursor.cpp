#include "stdafx.h"
#include "cursor.h"

#include "menuitem.h"

CCursor::CCursor( LPDIRECT3DTEXTURE8 Texture )
	: CSprite( Texture, D3DXVECTOR2(1.0f/16, 1.0f/16), 0, 
		D3DXVECTOR2( BOARD_W/2, BOARD_H/2 ), 0xFFFFFFFF )
{
}


CCursor::~CCursor()
{
}

HRESULT CCursor::Move( const D3DXVECTOR2 & vMovement )
{
	vPosition += vMovement;
	D3DXVec2Maximize( &vPosition, &vPosition, &D3DXVECTOR2( 0.00f, 0.00f ) );
	D3DXVec2Minimize( &vPosition, &vPosition, &D3DXVECTOR2( 1.00f, 0.75f ) );

	return S_OK;
}

void CCursor::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2) * RES_X;
	Position = D3DXVECTOR2( floor( Position.x ), floor( Position.y ) );
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &Position, dwBlending );
}
