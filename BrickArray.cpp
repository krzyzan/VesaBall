#include "StdAfx.h"
#include "BrickArray.h"

CBrick::STypeDesc CBrick::s_TypeDesc[CBrick::TYPE_MAX] = { 
	{1			}, 
	{1			}, 
	{1			},
	{0xFFFFFFFF	}, 
	{3			},
};


CBrickArray::CBrickArray()
{
	vSize = D3DXVECTOR2( 0.95f, 0.50f );
	vPosition = D3DXVECTOR2( 0.50f, 0.30f );
	
	D3DSURFACE_DESC sd;
	CBrick::s_TypeDesc->pTexture[0]->GetLevelDesc(0, &sd);
	vBrickSize = D3DXVECTOR2( vSize.x/MAX_X, vSize.y/MAX_Y );
	vBrickScaling = D3DXVECTOR2( vBrickSize.x/sd.Width, vBrickSize.y/sd.Height ) * RES_X;

	ZeroMemory( NewBrick, sizeof( NewBrick ) );
}


CBrickArray::~CBrickArray()
{
}

void CBrickArray::CreateBrick( const POINT & pos, LONG type )
{
	NewBrick[pos.x][pos.y].pTypeDesc = &CBrick::s_TypeDesc[type];
}

void CBrickArray::Render( LPD3DXSPRITE pSprite ) const
{
	for (int y=0; y<MAX_Y; y++)
		for (int x=0; x<MAX_X; x++)
			if (NewBrick[x][y].pTypeDesc) {
				D3DXVECTOR2 Position = (vPosition - vSize/2 + D3DXVECTOR2( vBrickSize.x*x, vBrickSize.y*y ) ) * RES_X;
				if (NewBrick[x][y].dwHitCounter < NewBrick[x][y].pTypeDesc->dur)					//TODO: ugly
					pSprite->Draw( NewBrick[x][y].pTypeDesc->pTexture[NewBrick[x][y].dwHitCounter], NULL, &vBrickScaling, NULL, 0, &Position, 0xFFFFFFFF );
			}
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
				NewBrick[pos.x][pos.y].pTypeDesc = NULL;
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
			if (NewBrick[x][y].pTypeDesc)
				file.put( char(CBrick::s_TypeDesc - NewBrick[x][y].pTypeDesc) + 48 );
			else
				file.put( ' ' );
		}
		file.put( '\n' );
	}
	file.close();

	return S_OK;
};

void CBrickArray::ZapBricks() 
{
	for (int x=0; x<MAX_X; x++)
		for (int y=0; y<MAX_Y; y++)
			if (NewBrick[x][y].pTypeDesc && NewBrick[x][y].pTypeDesc->dur != 0xFFFFFFFF )
				NewBrick[x][y].dwHitCounter = NewBrick[x][y].pTypeDesc->dur-1;
}


void CBrickArray::FallBricks() 
{
	for (int x=0; x<MAX_X; x++)
		for (int y=MAX_Y-2; y>=0; y--)
			if (NewBrick[x][y].pTypeDesc && NewBrick[x][y].pTypeDesc->dur != 0xFFFFFFFF && !NewBrick[x][y+1].pTypeDesc ) {
				NewBrick[x][y+1] = NewBrick[x][y];
				NewBrick[x][y].pTypeDesc = NULL;
				//NewBrick[x][y+1].vPosition.y += vBrickSize.y;
			}
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
