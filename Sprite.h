// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx8.h>
#include <list>

using namespace std;

class CBall;		//TMP

class CSprite  
{
public:
	CSprite( LPDIRECT3DTEXTURE8 Texture, D3DXVECTOR2 Scaling, 
			FLOAT Rotation, D3DXVECTOR2 Position, D3DCOLOR Blending );
	virtual ~CSprite();

	virtual void Render( LPD3DXSPRITE pSprite );
	virtual void FrameMove( FLOAT fElapsedTime ) = 0;
	virtual void Collide( list<CBall*>* pListBall );

	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;
	D3DXVECTOR2			vRotationCenter;
	FLOAT				fRotation;
	D3DXVECTOR2			vPosition;
	D3DCOLOR			dwBlending;

	BOOL				bDeleteMe;

	D3DXVECTOR2			vSize;
};
