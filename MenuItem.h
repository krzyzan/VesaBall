#pragma once
#include "sprite.h"

class CMenuItem :
	public CSprite
{
public:
	CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID );
	virtual ~CMenuItem();

	void Press();
	void Render( LPD3DXSPRITE pSprite ) const;

	HRESULT dwUID;
	BOOL bPressed;

private:
	CSprite* pShadow;
};
