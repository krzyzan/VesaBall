#include "StdAfx.h"
#include "GameBoard.h"
#include "Brick.h"

#include <fstream>		//TODO: TMP

CGameBoard::CGameBoard( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{
	pSprite			= NULL;
	ZeroMemory( pBrickTable, sizeof( pBrickTable ) );
}


CGameBoard::~CGameBoard()
{
}


HRESULT CGameBoard::InitDeviceObjects()
{
	LoadTexture( "gfx/Wall.png", &pWallTex );

	// tekstury cegie³ek
	LoadTexture( "gfx/Brick_normal.png",&pBrickTex[0] );
	LoadTexture( "gfx/Brick_normal.png",&pBrickTex[1] );
	LoadTexture( "gfx/Brick_normal.png",&pBrickTex[2] );
	LoadTexture( "gfx/Brick_metal.png",	&pBrickTex[3] );
	
	// sceneria
	listRender.push_back( new CSprite( pWallTex, D3DXVECTOR2(BOARD_L-0.0f, 0.75f), 0, D3DXVECTOR2((BOARD_L+0.00f)/2, 0.75f/2), 0xFFFFFFFF ) );
	listRender.push_back( new CSprite( pWallTex, D3DXVECTOR2(1.0f-BOARD_R, 0.75f), 0, D3DXVECTOR2((1.00f+BOARD_R)/2, 0.75f/2), 0xFFFFFFFF ) );

	// tworzymy cegie³ki

/*
	for (int x=0; x<BRICK_X; x++)
		for (int y=0; y<BRICK_Y; y++)
			if (rand()%4>0) {
				pBrickTable[x][y] = new CBrick( rand()%BRICK_TYPES, pBrickTex, D3DXVECTOR2( BOARD_L + BOARD_W/BRICK_X*(0.5f + x), BOARD_T + BRICK_TABLE_T + BRICK_TABLE_H/BRICK_Y*(0.5f + y) ), 
					D3DXVECTOR2( BOARD_W/BRICK_X, BRICK_TABLE_H/BRICK_Y) );
				InsertBrick( pBrickTable[x][y] );
			}
*/

	LoadLevel( "lev/level.lev" );

	return S_OK;
}


HRESULT CGameBoard::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}

HRESULT CGameBoard::RenderLoop()
{
	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

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
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}

// UWAGA: brak obs³ugi b³êdów
HRESULT CGameBoard::LoadLevel( char* strFileName )
{
	POINT pos;
	char ch;

	ifstream file;
	file.open( strFileName, ios::binary | ios::in );
	for (pos.y=0; pos.y<BRICK_Y; pos.y++) {
		for (pos.x=0; pos.x<BRICK_X; pos.x++) {
			file.get( ch );
			if (ch != ' ')
				InsertBrick( ch - 48, pos );
			else
				pBrickTable[pos.x][pos.y] = NULL;
		}
		file.get( ch );
	}
	file.close();

	return S_OK;
};


HRESULT CGameBoard::SaveLevel( char* strFileName )
{
	ofstream file;
	file.open( strFileName, ios::binary | ios::out | ios::trunc );
	for (int y=0; y<BRICK_Y; y++) {
		for (int x=0; x<BRICK_X; x++) {
			if (pBrickTable[x][y])
				file.put( pBrickTable[x][y]->dwType + 48 );
			else
				file.put( ' ' );
		}
		file.put( '\n' );
	}
	file.close();

	return S_OK;
};

void CGameBoard::InsertBrick( BYTE type, const POINT & pos )
{
	pBrickTable[pos.x][pos.y] = new CBrick( type, pBrickTex, 
		D3DXVECTOR2( BOARD_L + BOARD_W/BRICK_X*(0.5f + pos.x), BOARD_T + BRICK_TABLE_T + BRICK_TABLE_H/BRICK_Y*(0.5f + pos.y) ), 
		D3DXVECTOR2( BOARD_W/BRICK_X, BRICK_TABLE_H/BRICK_Y) );
	listRender.push_front( pBrickTable[pos.x][pos.y] );
}

void CGameBoard::RemoveBrick( const POINT & pos )
{
	if (pBrickTable[pos.x][pos.y] == NULL)
		return;
	
	listRender.remove( pBrickTable[pos.x][pos.y] );
	delete pBrickTable[pos.x][pos.y];
	pBrickTable[pos.x][pos.y] = NULL;
}
