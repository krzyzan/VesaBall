// Object.h: interface for the CObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list>		//TODO: TMP
using namespace std;

#include <d3dx8.h>


class CObject  
{
public:
	CObject( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
				 float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Blending );
	virtual ~CObject();

	virtual void Render( LPD3DXSPRITE pSprite ) const;

	void SetSize( const D3DXVECTOR2 & Size );

	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;			//TODO: private?
	D3DXVECTOR2			vRotationCenter;
	FLOAT				fRotation;
	D3DXVECTOR2			vPosition;
	D3DCOLOR			dwBlending;

	BOOL				bDeleteMe;
	D3DXVECTOR2			vSize;
};
