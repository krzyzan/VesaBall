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

	D3DXVECTOR2			vScaling;		//TODO: protected

	D3DXVECTOR2			vPosition;
	D3DXVECTOR2			vSize;
	D3DCOLOR			dwBlending;

	BOOL				bDeleteMe;

protected:
	LPDIRECT3DTEXTURE8	pTexture;
	FLOAT				fRotation;
	D3DXVECTOR2			vRotationCenter;
};
