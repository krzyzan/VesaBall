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
	for (LONG y=0; y<BRICK_ARRAY_Y; y++)
		for (LONG x=0; x<BRICK_ARRAY_X; x++)
			SAFE_DELETE( pBrick[x][y] );
}


void CBrickArray::ZapBricks() 
{
	for (int x=0; x<BRICK_ARRAY_X; x++)
		for (int y=0; y<BRICK_ARRAY_Y; y++)
			if (pBrick[x][y])
				pBrick[x][y]->Zap();
}


void CBrickArray::FallBricks() 
{
	for (int x=0; x<BRICK_ARRAY_X; x++)
		for (int y=BRICK_ARRAY_Y-2; y>=0; y--)
			if (pBrick[x][y] && pBrick[x][y]->IsDestructible() && !pBrick[x][y+1] ) {
				pBrick[x][y+1] = pBrick[x][y];
				pBrick[x][y] = NULL;
				pBrick[x][y+1]->vPosition.y += pBrick[x][y+1]->vSize.y;
			}
}


void CBrickArray::Render( LPD3DXSPRITE pSprite ) const
{
	for (int y=0; y<BRICK_ARRAY_Y; y++)
		for (int x=0; x<BRICK_ARRAY_X; x++)
			if (pBrick[x][y])
				pBrick[x][y]->Render( pSprite );
}


void CBrickArray::InsertBrick( DWORD type, const POINT & pos )
{
	pBrick[pos.x][pos.y] = new CBrick( type, 
		D3DXVECTOR2( vPosition.x - vSize.x/2 + vSize.x/BRICK_ARRAY_X*(0.5f + pos.x), vPosition.y - vSize.y/2 + vSize.y/BRICK_ARRAY_Y*(0.5f + pos.y) ), 
		D3DXVECTOR2( vSize.x/BRICK_ARRAY_X, vSize.y/BRICK_ARRAY_Y) );
}


bool CBrickArray::Contains( const D3DXVECTOR2 & vPos ) const
{
	return	fabs(vPosition.y - vPos.y) < vSize.y/2 &&
			fabs(vPosition.x - vPos.x) < vSize.x/2;
}


POINT CBrickArray::GetArrayCoords( const D3DXVECTOR2 & vPos ) const
{
	POINT pos;
	pos.x = (LONG)(((vPos.x - vPosition.x) / vSize.x + 0.5f) * BRICK_ARRAY_X);
	pos.y = (LONG)(((vPos.y - vPosition.y) / vSize.y + 0.5f) * BRICK_ARRAY_Y);
	return pos;
}


void CBrickArray::Load( const char* strFileName )
{
	ifstream file;
	file.open( strFileName, ios::binary | ios::in );

	char ch;
	POINT pos;
	for (pos.y=0; pos.y<BRICK_ARRAY_Y; pos.y++) {
		for (pos.x=0; pos.x<BRICK_ARRAY_X; pos.x++) {
			file.get( ch );
			if (ch != ' ')
				InsertBrick( ch - 48, pos );
			else
				pBrick[pos.x][pos.y] = NULL;
		}
		file.get( ch );
	}
	file.close();
};


void CBrickArray::Save( const char* strFileName ) const
{
	ofstream file;
	file.open( strFileName, ios::binary | ios::out | ios::trunc );
	for (int y=0; y<BRICK_ARRAY_Y; y++) {
		for (int x=0; x<BRICK_ARRAY_X; x++) {
			if (pBrick[x][y])
				file.put( (char)pBrick[x][y]->GetType() + 48 );
			else
				file.put( ' ' );
		}
		file.put( '\n' );
	}
	file.close();
};
