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

	bDeleteMe		= FALSE;
}

CSprite::~CSprite()
{

}

void CSprite::Render( LPD3DXSPRITE pSprite )
{
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &vPosition, dwBlending );
}
