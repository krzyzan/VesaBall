// Object.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <d3dx8.h>


class CSprite  
{
public:
	CSprite( const LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
			float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Blending );
	virtual ~CSprite();
	virtual void Render( LPD3DXSPRITE pSprite ) const;

	void SetSize( const D3DXVECTOR2 & Size );
	void FlipV() { vScaling.x*=-1; }
	void FlipH() { vScaling.y*=-1; }

	D3DXVECTOR2			vSize;
	D3DXVECTOR2			vPosition;
	D3DCOLOR			dwBlending;

protected:
	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;
	float				fRotation;
	D3DXVECTOR2			vRotationCenter;
};
