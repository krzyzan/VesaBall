#include "stdafx.h"
#include "gamemenu.h"

CGameMenu::CGameMenu( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppStage( d3dDevice, DIDevice )
{
	ZeroMemory( pTex, sizeof(pTex) );
	pSprite		= NULL;

	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Cursor_arrow.png",	&pTex[0]  );
}


HRESULT CGameMenu::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CGameMenu::FrameMove( FLOAT fElapsedTime )
{
	return S_OK;
}


HRESULT CGameMenu::Render()
{
	pd3dDevice->BeginScene();
	pSprite->Begin();

	pSprite->End();
	pd3dDevice->EndScene();


	return S_OK;
}


HRESULT CGameMenu::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );
	return S_OK;
}


CGameMenu::~CGameMenu()
{


	for (int i=0; i<256; i++)
		SAFE_RELEASE( pTex[i] );
}
