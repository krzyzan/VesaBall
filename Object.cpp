// Sprite.cpp: implementation of the CObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Object.h"

#include "Ball.h"	//TODO: TMP

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObject::CObject( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
				 float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Blending )
{
	pTexture		= Texture;
	SetSize( Size );
	fRotation		= Rotation;
	vPosition		= Position;
	dwBlending		= Blending;
    
	bDeleteMe		= FALSE;
}

CObject::~CObject()
{
}

void CObject::SetSize( const D3DXVECTOR2 & Size )
{
	vSize			= Size;
	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling		= D3DXVECTOR2( vSize.x/sd.Width, vSize.y/sd.Height ) * RES_X;
	vRotationCenter = vSize * RES_X / 2;
}

void CObject::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2);
	if (vScaling.x < 0) Position.x += vSize.x;
	if (vScaling.y < 0) Position.y += vSize.y;
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &(Position * RES_X), dwBlending );
}
