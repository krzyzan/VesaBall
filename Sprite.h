// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;
#include <list>		//TODO: TMP
#include <d3dx8.h>

class CBall;		//TODO: TMP


class CSprite  
{
public:
	CSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
				 float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Blending );
	virtual ~CSprite();

	virtual void Render( LPD3DXSPRITE pSprite ) const;
	virtual void BallHits( CBall* pBall, const D3DXVECTOR2 & vSide );

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
