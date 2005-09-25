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


HRESULT CGameBoard::OnInitDevice()
{
	// tekstury cegie³ek
	LoadTexture( "gfx/Brick_red.png",		&CBrick::spTexture[ 1] );
	LoadTexture( "gfx/Brick_metal.png",		&CBrick::spTexture[ 2] );
	LoadTexture( "gfx/Brick_grey0.png",		&CBrick::spTexture[ 3] );
	LoadTexture( "gfx/Brick_grey1.png",		&CBrick::spTexture[ 4] );
	LoadTexture( "gfx/Brick_grey2.png",		&CBrick::spTexture[ 5] );
	LoadTexture( "gfx/Brick_trans0.png",	&CBrick::spTexture[ 6] );
	LoadTexture( "gfx/Brick_trans1.png",	&CBrick::spTexture[ 7] );
	LoadTexture( "gfx/Brick_expl.png",		&CBrick::spTexture[ 8] );
	LoadTexture( "gfx/Brick_green.png",		&CBrick::spTexture[ 9] );
	LoadTexture( "gfx/Brick_blue.png",		&CBrick::spTexture[10] );
	LoadTexture( "gfx/Brick_violet.png",	&CBrick::spTexture[11] );
	LoadTexture( "gfx/Brick_cyan.png",		&CBrick::spTexture[12] );
	LoadTexture( "gfx/Brick_orange.png",	&CBrick::spTexture[13] );
	LoadTexture( "gfx/Brick_pinkd.png",		&CBrick::spTexture[14] );
	LoadTexture( "gfx/Brick_pink.png",		&CBrick::spTexture[15] );
	LoadTexture( "gfx/Brick_purple.png",	&CBrick::spTexture[16] );
	LoadTexture( "gfx/Brick_marine.png",	&CBrick::spTexture[17] );
	LoadTexture( "gfx/Brick_greend.png",	&CBrick::spTexture[18] );
	LoadTexture( "gfx/Brick_olive.png",		&CBrick::spTexture[19] );
	LoadTexture( "gfx/Brick_brown.png",		&CBrick::spTexture[20] );
	LoadTexture( "gfx/Brick_metalp.png",	&CBrick::spTexture[21] );
	LoadTexture( "gfx/Brick_bluel.png",		&CBrick::spTexture[22] );
	

	// sceneria
	LPDIRECT3DTEXTURE8 pWallTex;
	LoadTexture( "gfx/Wall.png", &pWallTex );
	listSprite.push_back( new CSprite( pWallTex, D3DXVECTOR2(BOARD_L-0.0f, 0.75f), 0, D3DXVECTOR2((BOARD_L+0.00f)/2, 0.75f/2), 0xFFFFFFFF ) );
	listSprite.push_back( new CSprite( pWallTex, D3DXVECTOR2(1.0f-BOARD_R, 0.75f), 0, D3DXVECTOR2((1.00f+BOARD_R)/2, 0.75f/2), 0xFFFFFFFF ) );

	// tworzymy cegie³ki
	POINT arraySize = {20, 20};
	pBrickArray	= new CBrickArray( arraySize, D3DXVECTOR2( BOARD_L+BOARD_W/2, 0.30f ), D3DXVECTOR2( BOARD_W, BOARD_W/2 ) );
	pBrickArray->Load( dwLevelNum );

	return S_OK;
}


HRESULT CGameBoard::OnRestoreDevice()
{
	D3DXCreateSprite( pD3DDevice, &pSprite );

	return S_OK;
}

HRESULT CGameBoard::OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod )
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


HRESULT CGameBoard::OnInvalidateDevice()
{
	SAFE_RELEASE( pSprite );

	return S_OK;
}


HRESULT CGameBoard::OnDeleteDevice()
{
	// Kasujemy z listy renderowania
	list<CSprite*>::iterator iSprite;
	for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
		delete (*iSprite++);

	delete pBrickArray;

	return S_OK;
}
