#include "StdAfx.h"
#include "MenuItem.h"

const Vec2 SHADOW_POS = Vec2(0.005f, 0.005f);

CMenuItem::CMenuItem(SDL_Texture* Texture, const Vec2 & Size,
					 const Vec2 & Position, Color Color, HRESULT UID)
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

void CMenuItem::Render(SDL_Renderer* pRenderer) const
{
	pShadow->Render(pRenderer);
	CSprite::Render(pRenderer);
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
