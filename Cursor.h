#pragma once

#include <dinput.h>
#include "Sprite.h"

const float MOUSE_SPEED = 0.002f;

class CCursor :
	public CSprite
{
public:
	CCursor( LPDIRECT3DTEXTURE8 Texture );
	virtual ~CCursor();

	HRESULT Move( const D3DXVECTOR2 & vMovement );
	void Render( LPD3DXSPRITE pSprite ) const;
};
