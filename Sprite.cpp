// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite( LPDIRECT3DTEXTURE8 Texture, D3DXVECTOR2 Scaling, 
				 D3DXVECTOR2 RotationCenter, float Rotation, 
				 D3DXVECTOR2 Position, D3DCOLOR Blending )
{
	pTexture		= Texture;
	vScaling		= Scaling;
	vRotationCenter	= RotationCenter;
	fRotation		= Rotation;
	vPosition		= Position;
	dwBlending		= Blending;


	D3DSURFACE_DESC SurfaceDesc;
	pTexture->GetLevelDesc(0, &SurfaceDesc);

	vTexSize.x = (FLOAT)SurfaceDesc.Width;
	vTexSize.y = (FLOAT)SurfaceDesc.Height;

	vTexSize.x *= vScaling.x;
	vTexSize.y *= vScaling.y;

	bDeleteMe		= FALSE;
}

CSprite::~CSprite()
{

}

void CSprite::Render( LPD3DXSPRITE pSprite )
{
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &(vPosition-vTexSize/2), dwBlending );
}
