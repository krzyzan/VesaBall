#include "stdafx.h"
#include "menuitem.h"

CMenuItem::CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		 const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID )
	: CSprite( Texture, Size, 0, Position, Blending )
{
	dwUID = UID;
	bPressed = false;

	pShadow = new CSprite( *this );
	pShadow->dwBlending = 0x3F000000;
	pShadow->vPosition += D3DXVECTOR2(0.005f, 0.005f);
}

CMenuItem::~CMenuItem()
{
	delete pShadow;
}

void CMenuItem::Render( LPD3DXSPRITE pSprite ) const
{
	if (!bPressed)
		pShadow->Render( pSprite );

	CSprite::Render( pSprite );
}

void CMenuItem::Press()
{
	if (!bPressed)
	{
		bPressed = true;
		vPosition += D3DXVECTOR2(0.005f, 0.005f);
	}
}
