#pragma once
#include "Sprite.h"

const float SHADOW_DISTANCE = 0.005f;

class CMenuItem :
	public CSprite
{
public:
	CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, D3DCOLOR Blending, HRESULT UID );
	virtual ~CMenuItem();

	void Render( LPD3DXSPRITE pSprite ) const;
	
	void SetPressed( BOOL bPressed );
	void SetHighlighted( BOOL bHighlighted );
	HRESULT GetUID() const { return dwUID; }

private:
	CSprite* pShadow;
	HRESULT dwUID;
};
