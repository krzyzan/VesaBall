#include "stdafx.h"
#include "menuitem.h"

CMenuItem::CMenuItem( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		 const D3DXVECTOR2 & Position, D3DCOLOR Blending )
	: CSprite( Texture, Size, 0, Position, Blending )
{
}

CMenuItem::~CMenuItem()
{
}
