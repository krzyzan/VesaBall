#include "StdAfx.h"
#include "GameBoard.h"
#include "BrickArray.h"


CGameBoard::CGameBoard()
{
	pSprite		= NULL;
	dwLevelNum	= 0;
}


CGameBoard::~CGameBoard()
{
}


HRESULT CGameBoard::InitDeviceObjects()
{
	// tekstury cegie³ek
	LoadTexture( "gfx/Brick_red.png",	&CBrick::spTexture[ 0][0] );
	LoadTexture( "gfx/Brick_orange.png",&CBrick::spTexture[ 1][0] );
	LoadTexture( "gfx/Brick_yellow.png",&CBrick::spTexture[ 2][0] );
	LoadTexture( "gfx/Brick_olive.png",	&CBrick::spTexture[ 3][0] );
	LoadTexture( "gfx/Brick_green.png",	&CBrick::spTexture[ 4][0] );
	LoadTexture( "gfx/Brick_marine.png",&CBrick::spTexture[ 5][0] );
	LoadTexture( "gfx/Brick_cyan.png",	&CBrick::spTexture[ 6][0] );
	LoadTexture( "gfx/Brick_blue.png",	&CBrick::spTexture[ 7][0] );
	LoadTexture( "gfx/Brick_violet.png",&CBrick::spTexture[ 8][0] );
	LoadTexture( "gfx/Brick_purple.png",&CBrick::spTexture[ 9][0] );
	LoadTexture( "gfx/Brick_metal.png",	&CBrick::spTexture[10][0] );

	LoadTexture( "gfx/Brick_0.png",		&CBrick::spTexture[11][0] );
	LoadTexture( "gfx/Brick_1.png",		&CBrick::spTexture[11][1] );
	LoadTexture( "gfx/Brick_2.png",		&CBrick::spTexture[11][2] );
	
	LoadTexture( "gfx/Brick_trans0.png",&CBrick::spTexture[12][0] );
	LoadTexture( "gfx/Brick_trans1.png",&CBrick::spTexture[12][1] );

	// sceneria
	LPDIRECT3DTEXTURE8 pWallTex;
	LoadTexture( "gfx/Wall.png", &pWallTex );
	listSprite.push_back( new CSprite( pWallTex, D3DXVECTOR2(BOARD_L-0.0f, 0.75f), 0, D3DXVECTOR2((BOARD_L+0.00f)/2, 0.75f/2), 0xFFFFFFFF ) );
	listSprite.push_back( new CSprite( pWallTex, D3DXVECTOR2(1.0f-BOARD_R, 0.75f), 0, D3DXVECTOR2((1.00f+BOARD_R)/2, 0.75f/2), 0xFFFFFFFF ) );

	// tworzymy cegie³ki
	pBrickArray	= new CBrickArray( D3DXVECTOR2( BOARD_L+BOARD_W/2, 0.30f ), D3DXVECTOR2( BOARD_W, BOARD_W/2 ) );
	pBrickArray->Load( dwLevelNum );

	return S_OK;
}


HRESULT CGameBoard::RestoreDeviceObjects()
{
	D3DXCreateSprite( pD3DDevice, &pSprite );

	return S_OK;
}

HRESULT CGameBoard::ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )
{
	if (didod->dwOfs == DIK_ESCAPE && (didod->dwData & 0x80) )
		SetCurrentScene( NULL );

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
