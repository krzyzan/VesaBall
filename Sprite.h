// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <d3dx8.h>
#include <list>		//TMP

using namespace std;

class CBall;		//TMP
class CLevel;

class CSprite  
{
public:
	CSprite( CLevel* Level, LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
				 float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Blending );
	virtual ~CSprite();

	virtual void Render( LPD3DXSPRITE pSprite ) const;
	virtual void Collide( list<CBall*>* pListBall );	//TMP

	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;
	D3DXVECTOR2			vRotationCenter;
	FLOAT				fRotation;
	D3DXVECTOR2			vPosition;
	D3DCOLOR			dwBlending;

	BOOL				bDeleteMe;
	D3DXVECTOR2			vSize;

	CLevel*				pLevel;
};
