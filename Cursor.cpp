#include "StdAfx.h"
#include "Cursor.h"

const float MOUSE_SPEED = 0.002f;

CCursor::CCursor(LPDIRECT3DTEXTURE8 Texture)
	: CSprite(Texture, D3DXVECTOR2(1.0f / 16, 1.0f / 16), 0,
			  D3DXVECTOR2(1.0f / 2, 0.75f / 2), 0xFFFFFFFF)
{
}

CCursor::~CCursor()
{
}

HRESULT CCursor::Move(const D3DXVECTOR2 & vMovement)
{
	vPosition += vMovement * MOUSE_SPEED;
	D3DXVECTOR2 vTopLeft(0.00f, 0.00f);
	D3DXVECTOR2 vBottomRight(1.00f, 0.75f);
	D3DXVec2Maximize(&vPosition, &vPosition, &vTopLeft);
	D3DXVec2Minimize(&vPosition, &vPosition, &vBottomRight);

	return S_OK;
}

void CCursor::Render(LPD3DXSPRITE pSprite) const
{
	D3DXVECTOR2 Position = (vPosition - vSize / 2) * (float)RES_X;
	Position = D3DXVECTOR2(floor(Position.x), floor(Position.y));
	pSprite->Draw(pTexture, NULL, &vScaling, &vRotationCenter, fRotation, &Position, dwColor);
}
