// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprite.h"
#include "Ball.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite( LPDIRECT3DTEXTURE8 Texture, D3DXVECTOR2 Size, 
				 float Rotation, D3DXVECTOR2 Position, D3DCOLOR Blending )
{
	pTexture		= Texture;
	vSize			= Size;

	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling		= D3DXVECTOR2( vSize.x/sd.Width, vSize.y/sd.Height ) * RES_X;
	vRotationCenter = vSize * RES_X / 2;

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
	pSprite->Draw( pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &((vPosition-vSize/2)*RES_X), dwBlending );
}

void CSprite::FrameMove( FLOAT fElapsedTime )
{

}

void CSprite::Collide( list<CBall*>* pListBall )
{

}