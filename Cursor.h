#pragma once

#include <dinput.h>
#include "Sprite.h"


class CCursor :
	public CSprite
{
public:
	CCursor( LPDIRECT3DTEXTURE8 Texture );
	virtual ~CCursor();

	HRESULT MouseMove( DIMOUSESTATE2* dims2 );
	void Render( LPD3DXSPRITE pSprite ) const;
};
