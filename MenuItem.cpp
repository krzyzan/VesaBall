#include "stdafx.h"
#include "menuitem.h"

CMenuItem::CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		 const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID )
	: CSprite( Texture, Size, 0, Position, Blending )
{
	dwUID = UID;

	pShadow = new CSprite( *this );
	pShadow->dwBlending = 0x2F000000;
	pShadow->vPosition += D3DXVECTOR2( SHADOW_DISTANCE, SHADOW_DISTANCE );
}

CMenuItem::~CMenuItem()
{
	delete pShadow;
}

void CMenuItem::Render( LPD3DXSPRITE pSprite ) const
{
	pShadow->Render( pSprite );
	CSprite::Render( pSprite );
}

void CMenuItem::SetPressed( BOOL bPressed )
{
	vPosition += bPressed ? 
		D3DXVECTOR2(  SHADOW_DISTANCE/2,  SHADOW_DISTANCE/2 ) : 
		D3DXVECTOR2( -SHADOW_DISTANCE/2, -SHADOW_DISTANCE/2 );
}

void CMenuItem::SetHighlighted( BOOL bHighlighted )
{
	if (bHighlighted)
		dwBlending |= ~0xFFFFCC00;
	else
		dwBlending &=  0xFFFFCC00;
}
