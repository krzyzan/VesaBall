// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite( const LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
				 float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Blending )
{
	pTexture		= Texture;
	SetSize( Size );
	fRotation		= Rotation;
	vPosition		= Position;
	dwBlending		= Blending;
}

CSprite::~CSprite()
{
}

void CSprite::SetSize( const D3DXVECTOR2 & Size )
{
	vSize			= Size;
	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling		= D3DXVECTOR2( vSize.x/sd.Width, vSize.y/sd.Height ) * RES_X;
	vRotationCenter = vSize * RES_X / 2;
}

void CSprite::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2);
	if (vScaling.x < 0) Position.x += vSize.x;
	if (vScaling.y < 0) Position.y += vSize.y;
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &(Position * RES_X), dwBlending );
}
