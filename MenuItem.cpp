#include "StdAfx.h"
#include "MenuItem.h"

const D3DXVECTOR2 SHADOW_POS = D3DXVECTOR2(0.005f, 0.005f);

CMenuItem::CMenuItem(LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size,
					 const D3DXVECTOR2 & Position, D3DCOLOR Color, HRESULT UID)
	: CSprite(Texture, Size, 0, Position, Color)
{
	dwUID = UID;

	pShadow = new CSprite(*this);
	pShadow->SetColor(0x2F000000);
	pShadow->vPosition += SHADOW_POS;
}

CMenuItem::~CMenuItem()
{
	delete pShadow;
}

void CMenuItem::Render(LPD3DXSPRITE pSprite) const
{
	pShadow->Render(pSprite);
	CSprite::Render(pSprite);
}

void CMenuItem::SetPressed(bool bPressed)
{
	vPosition += bPressed ? SHADOW_POS / 2 : -SHADOW_POS / 2;
}

void CMenuItem::SetHighlighted(bool bHighlighted)
{
	if (bHighlighted)
		dwColor |= ~0xFFFFCC00;
	else
		dwColor &= 0xFFFFCC00;
}
