#include "StdAfx.h"
#include "BrickArray.h"


CBrickArray::CBrickArray()
{
	vSize = D3DXVECTOR2( 0.95f, 0.50f );
	vPosition = D3DXVECTOR2( 0.50f, 0.30f );

	ZeroMemory( pBrick, sizeof( pBrick ) );
}


CBrickArray::~CBrickArray()
{
	for (LONG y=0; y<MAX_Y; y++)
		for (LONG x=0; x<MAX_X; x++)
			SAFE_DELETE( pBrick[x][y] );
}


void CBrickArray::ZapBricks() 
{
	for (int x=0; x<MAX_X; x++)
		for (int y=0; y<MAX_Y; y++)
			if (pBrick[x][y] && pBrick[x][y]->pTypeDesc->dur != 0xFFFFFFFF )
				pBrick[x][y]->dwHitCounter = pBrick[x][y]->pTypeDesc->dur-1;
}


void CBrickArray::FallBricks() 
{
	for (int x=0; x<MAX_X; x++)
		for (int y=MAX_Y-2; y>=0; y--)
			if (pBrick[x][y] && pBrick[x][y]->pTypeDesc->dur != 0xFFFFFFFF && !pBrick[x][y+1] ) {
				pBrick[x][y+1] = pBrick[x][y];
				pBrick[x][y] = NULL;
				pBrick[x][y+1]->vPosition.y += pBrick[x][y+1]->vSize.y;
			}
}


void CBrickArray::Render( LPD3DXSPRITE pSprite ) const
{
	for (int y=0; y<MAX_Y; y++)
		for (int x=0; x<MAX_X; x++)
			if (pBrick[x][y])
				pBrick[x][y]->Render( pSprite );
}


void CBrickArray::InsertBrick( BYTE type, const POINT & pos )
{
	pBrick[pos.x][pos.y] = new CBrick( &CBrick::s_TypeDesc[type], 
		D3DXVECTOR2( vPosition.x - vSize.x/2 + vSize.x/MAX_X*(0.5f + pos.x), vPosition.y - vSize.y/2 + vSize.y/MAX_Y*(0.5f + pos.y) ), 
		D3DXVECTOR2( vSize.x/MAX_X, vSize.y/MAX_Y) );
}


bool CBrickArray::Contains( const D3DXVECTOR2 & vPos ) const
{
	return	fabs(vPosition.y - vPos.y) < vSize.y/2 &&
			fabs(vPosition.x - vPos.x) < vSize.x/2;
}


POINT CBrickArray::VectorToArrayCoords( const D3DXVECTOR2 & vPos ) const
{
	POINT pos;
	pos.x = (LONG)(((vPos.x - vPosition.x) / vSize.x + 0.5f) * MAX_X);
	pos.y = (LONG)(((vPos.y - vPosition.y) / vSize.y + 0.5f) * MAX_Y);
	return pos;
}


// UWAGA: brak obs³ugi b³êdów
HRESULT CBrickArray::Load( char* strFileName )
{
	ifstream file;
	file.open( strFileName, ios::binary | ios::in );

	char ch;
	POINT pos;
	for (pos.y=0; pos.y<MAX_Y; pos.y++) {
		for (pos.x=0; pos.x<MAX_X; pos.x++) {
			file.get( ch );
			if (ch != ' ')
				InsertBrick( ch - 48, pos );
			else
				pBrick[pos.x][pos.y] = NULL;
		}
		file.get( ch );
	}
	file.close();

	return S_OK;
};


HRESULT CBrickArray::Save( char* strFileName ) const
{
	ofstream file;
	file.open( strFileName, ios::binary | ios::out | ios::trunc );
	for (int y=0; y<MAX_Y; y++) {
		for (int x=0; x<MAX_X; x++) {
			if (pBrick[x][y])
				file.put( pBrick[x][y]->pTypeDesc->type + 48 );
			else
				file.put( ' ' );
		}
		file.put( '\n' );
	}
	file.close();

	return S_OK;
};
