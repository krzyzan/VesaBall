// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprite.h"
#include "Ball.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite( LPDIRECT3DTEXTURE8 Texture, D3DXVECTOR2 Scaling, 
				 float Rotation, D3DXVECTOR2 Position, D3DCOLOR Blending )
{
	pTexture		= Texture;
	vScaling		= D3DXVECTOR2( Scaling.x*RES_X/1024, Scaling.y*RES_Y/768 );
	fRotation		= Rotation;
	vPosition		= Position;
	dwBlending		= Blending;


	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);

    vSize = D3DXVECTOR2((FLOAT)sd.Width*vScaling.x, (FLOAT)sd.Height*vScaling.y );
	vRotationCenter = vSize/2;
	vSize /= RES_X;

	bDeleteMe		= FALSE;
}

CSprite::~CSprite()
{

}

void CSprite::Render( LPD3DXSPRITE pSprite )
{
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &((vPosition-vSize/2)*RES_X), dwBlending );
}

void CSprite::Collide( list<CBall*>* pListBall )
{

}