#include "stdafx.h"
#include "menuitem.h"

CMenuItem::CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		 const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID )
	: CObject( Texture, Size, 0, Position, Blending )
{
	dwUID = UID;

	pShadow = new CObject( *this );
	pShadow->dwBlending = 0x2F000000;
	pShadow->vPosition += SHADOW_DISTANCE;
}

CMenuItem::~CMenuItem()
{
	delete pShadow;
}

void CMenuItem::Render( LPD3DXSPRITE pSprite ) const
{
	pShadow->Render( pSprite );
	CObject::Render( pSprite );
}

void CMenuItem::SetPressed( BOOL bPressed )
{
	if (bPressed)
		vPosition += SHADOW_DISTANCE/2;
	else
		vPosition -= SHADOW_DISTANCE/2;
}

void CMenuItem::SetHighlighted( BOOL bHighlighted )
{
	if (bHighlighted)
		dwBlending |= ~0xFFFFCC00;
	else
		dwBlending &=  0xFFFFCC00;
}
