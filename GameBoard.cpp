#include "StdAfx.h"
#include "GameBoard.h"
#include "BrickArray.h"

CGameBoard::CGameBoard( LPDIRECT3DDEVICE8 d3dDevice )
	: CD3DAppScene( d3dDevice )
{
	pSprite			= NULL;
}


CGameBoard::~CGameBoard()
{
}


HRESULT CGameBoard::InitDeviceObjects()
{
	// tekstury cegie³ek
	LoadTexture( "gfx/Brick_red.png",	&CBrick::s_pTexture[ 0][0] );
	LoadTexture( "gfx/Brick_orange.png",&CBrick::s_pTexture[ 1][0] );
	LoadTexture( "gfx/Brick_yellow.png",&CBrick::s_pTexture[ 2][0] );
	LoadTexture( "gfx/Brick_olive.png",	&CBrick::s_pTexture[ 3][0] );
	LoadTexture( "gfx/Brick_green.png",	&CBrick::s_pTexture[ 4][0] );
	LoadTexture( "gfx/Brick_marine.png",&CBrick::s_pTexture[ 5][0] );
	LoadTexture( "gfx/Brick_cyan.png",	&CBrick::s_pTexture[ 6][0] );
	LoadTexture( "gfx/Brick_blue.png",	&CBrick::s_pTexture[ 7][0] );
	LoadTexture( "gfx/Brick_violet.png",&CBrick::s_pTexture[ 8][0] );
	LoadTexture( "gfx/Brick_purple.png",&CBrick::s_pTexture[ 9][0] );
	LoadTexture( "gfx/Brick_metal.png",	&CBrick::s_pTexture[10][0] );

	LoadTexture( "gfx/Brick_0.png",		&CBrick::s_pTexture[11][0] );
	LoadTexture( "gfx/Brick_1.png",		&CBrick::s_pTexture[11][1] );
	LoadTexture( "gfx/Brick_2.png",		&CBrick::s_pTexture[11][2] );
	
	// sceneria
	LPDIRECT3DTEXTURE8 pWallTex;
	LoadTexture( "gfx/Wall.png", &pWallTex );
	listSprite.push_back( new CSprite( pWallTex, D3DXVECTOR2(BOARD_L-0.0f, 0.75f), 0, D3DXVECTOR2((BOARD_L+0.00f)/2, 0.75f/2), 0xFFFFFFFF ) );
	listSprite.push_back( new CSprite( pWallTex, D3DXVECTOR2(1.0f-BOARD_R, 0.75f), 0, D3DXVECTOR2((1.00f+BOARD_R)/2, 0.75f/2), 0xFFFFFFFF ) );

	// tworzymy cegie³ki
	pBrickArray	= new CBrickArray();
	pBrickArray->Load( "lev/level.lev" );

	return S_OK;
}


HRESULT CGameBoard::RestoreDeviceObjects()
{
	D3DXCreateSprite( pD3DDevice, &pSprite );

	return S_OK;
}

HRESULT CGameBoard::FrameRender()
{
	list<CSprite*>::iterator iSprite;
	for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pBrickArray->Render( pSprite );

	return S_OK;
}


HRESULT CGameBoard::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );

	return S_OK;
}


HRESULT CGameBoard::DeleteDeviceObjects()
{
	// Kasujemy z listy renderowania
	list<CSprite*>::iterator iSprite;
	for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
		delete (*iSprite++);

	delete pBrickArray;

	return S_OK;
}
