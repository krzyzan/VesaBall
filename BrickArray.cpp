#include "StdAfx.h"
#include "BrickArray.h"

CNewBrick::STypeDesc CNewBrick::s_TypeDesc[CNewBrick::TYPE_MAX] = { 
	{0,	1			}, 
	{1,	1			}, 
	{2, 1			},
	{3,	0xFFFFFFFF	}, 
	{4,	3			},
};


CBrickArray::CBrickArray()
{
	vSize = D3DXVECTOR2( 0.95f, 0.50f );
	vPosition = D3DXVECTOR2( 0.50f, 0.30f );

	ZeroMemory( NewBrick, sizeof( NewBrick ) );
}


CBrickArray::~CBrickArray()
{
	for (LONG y=0; y<MAX_Y; y++)
		for (LONG x=0; x<MAX_X; x++)
			SAFE_DELETE( NewBrick[x][y].pBrick );
}


void CBrickArray::ZapBricks() 
{
	for (int x=0; x<MAX_X; x++)
		for (int y=0; y<MAX_Y; y++)
			if (NewBrick[x][y].pBrick && NewBrick[x][y].pTypeDesc->dur != 0xFFFFFFFF )
				NewBrick[x][y].dwHitCounter = NewBrick[x][y].pTypeDesc->dur-1;
}


void CBrickArray::FallBricks() 
{
	for (int x=0; x<MAX_X; x++)
		for (int y=MAX_Y-2; y>=0; y--)
			if (NewBrick[x][y].pBrick && NewBrick[x][y].pTypeDesc->dur != 0xFFFFFFFF && !NewBrick[x][y+1].pBrick ) {
				NewBrick[x][y+1].pBrick = NewBrick[x][y].pBrick;
				NewBrick[x][y].pBrick = NULL;
				NewBrick[x][y+1].vPosition.y += NewBrick[x][y+1].vSize.y;
			}
}


void CBrickArray::Render( LPD3DXSPRITE pSprite ) const
{
	for (int y=0; y<MAX_Y; y++)
		for (int x=0; x<MAX_X; x++)
			if (NewBrick[x][y].pBrick) {
				D3DXVECTOR2 Position = (NewBrick[x][y].vPosition - NewBrick[x][y].vSize/2);
				if (NewBrick[x][y].dwHitCounter < NewBrick[x][y].pTypeDesc->dur)					//TODO: ugly
					pSprite->Draw( NewBrick[x][y].pTypeDesc->pTexture[NewBrick[x][y].dwHitCounter], NULL, &NewBrick[x][y].vScaling, NULL, 0, &(Position * RES_X), 0xFFFFFFFF );
			}
}


void CBrickArray::CreateBrick( const POINT & pos, BYTE type )
{
	NewBrick[pos.x][pos.y].pBrick = new CBrick();
	NewBrick[pos.x][pos.y].pTypeDesc = &CNewBrick::s_TypeDesc[type];
	NewBrick[pos.x][pos.y].vPosition = D3DXVECTOR2( vPosition.x - vSize.x/2 + vSize.x/MAX_X*(0.5f + pos.x), vPosition.y - vSize.y/2 + vSize.y/MAX_Y*(0.5f + pos.y) );
	NewBrick[pos.x][pos.y].SetSize( D3DXVECTOR2( vSize.x/MAX_X, vSize.y/MAX_Y) );
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
				CreateBrick( pos, ch - 48 );
			else
				NewBrick[pos.x][pos.y].pBrick = NULL;
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
			if (NewBrick[x][y].pBrick)
				file.put( NewBrick[x][y].pTypeDesc->type + 48 );
			else
				file.put( ' ' );
		}
		file.put( '\n' );
	}
	file.close();

	return S_OK;
};
