// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx8.h>

class CSprite  
{
public:
	CSprite( LPDIRECT3DTEXTURE8 Texture, D3DXVECTOR2 Scaling, 
				 D3DXVECTOR2 RotationCenter, float Rotation, 
				 D3DXVECTOR2 Position, D3DCOLOR Blending );
	virtual ~CSprite();

	virtual void Render( LPD3DXSPRITE pSprite );
	virtual void FrameMove() = 0;

	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;
	D3DXVECTOR2			vRotationCenter;
	FLOAT				fRotation;
	D3DXVECTOR2			vPosition;
	D3DCOLOR			dwBlending;

	BOOL				bDeleteMe;
};
