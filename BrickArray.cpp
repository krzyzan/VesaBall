#include "StdAfx.h"
#include "BrickArray.h"


CBrickArray::CBrickArray( const POINT & size, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & ScreenSize )
{
	vPosition = Position;
	vSize = ScreenSize;

	Max = size;

	pBrick = new CBrick**[Max.x];
	for (LONG x=0; x<Max.x; x++) {
		pBrick[x] = new CBrick*[Max.y];
		ZeroMemory( pBrick[x], sizeof(pBrick[0])*Max.y );
	}
	
	dwBricksLeft = 0;
}


CBrickArray::~CBrickArray()
{
	Clear();

	for (LONG x=0; x<Max.x; x++)
		delete [] pBrick[x];
	delete [] pBrick;
}


void CBrickArray::Clear()
{
	POINT pos;
	for (pos.y=0; pos.y<Max.y; pos.y++)
		for (pos.x=0; pos.x<Max.x; pos.x++)
			RemoveBrick( pos );
}


void CBrickArray::Render( LPD3DXSPRITE pSprite ) const
{
	for (int y=0; y<Max.y; y++)
		for (int x=0; x<Max.x; x++)
			if (pBrick[x][y])
				pBrick[x][y]->Render( pSprite );
}


void CBrickArray::InsertBrick( BYTE idType, const POINT & pos )
{
	pBrick[pos.x][pos.y] = new CBrick( idType, GetPositionAt( pos ),
		D3DXVECTOR2( vSize.x/Max.x, vSize.y/Max.y) );
	
	if ( pBrick[pos.x][pos.y]->MustHit() )
		dwBricksLeft++;
}


void CBrickArray::RemoveBrick( const POINT & pos ) 
{ 
	if ( !pBrick[pos.x][pos.y] )
		return;
	
	if ( pBrick[pos.x][pos.y]->MustHit() )
		dwBricksLeft--;
	
	SAFE_DELETE( pBrick[pos.x][pos.y] ); 
}


void CBrickArray::Load( DWORD dwLevelNum )
{
	Clear();

	char strFileName[MAX_PATH];
	sprintf( strFileName, "lev/%02d.lev", dwLevelNum );

	ifstream file;
	file.open( strFileName, ios::binary | ios::in );
	char ch;
	POINT pos;
	for (pos.y=0; pos.y<Max.y; pos.y++) {
		for (pos.x=0; pos.x<Max.x; pos.x++) {
			file.get( ch );
			if ( ch )
				InsertBrick( ch, pos );
		}
		file.get( ch );
	}
	file.close();
};


void CBrickArray::Save( DWORD dwLevelNum ) const
{
	char strFileName[MAX_PATH];
	sprintf( strFileName, "lev/%02d.lev", dwLevelNum );
	
	ofstream file;
	file.open( strFileName, ios::binary | ios::out | ios::trunc );
	for (int y=0; y<Max.y; y++) {
		for (int x=0; x<Max.x; x++) {
			if (pBrick[x][y])
				file.put( (char)pBrick[x][y]->GetType() );
			else
				file.put( '\0' );
		}
		file.put( '\n' );
	}
	file.close();
};

POINT CBrickArray::GetArrayCoordsAt( const D3DXVECTOR2 & vPos ) const
{
	POINT pos;
	pos.x = (LONG)(((vPos.x - vPosition.x) / vSize.x + 0.5f) * Max.x);
	pos.y = (LONG)(((vPos.y - vPosition.y) / vSize.y + 0.5f) * Max.y);
	return pos;
}


D3DXVECTOR2 CBrickArray::GetPositionAt( const POINT & pos ) const
{
	return vPosition - vSize/2 + D3DXVECTOR2( vSize.x/Max.x*(0.5f + pos.x), vSize.y/Max.y*(0.5f + pos.y) );
}

void CBrickArray::ZapBricks() 
{
	POINT pos;
	for (pos.x=0; pos.x<Max.x; pos.x++)
		for (pos.y=0; pos.y<Max.y; pos.y++) {
			if (!pBrick[pos.x][pos.y])
				continue;
			
			while (true) {
				BYTE idType = pBrick[pos.x][pos.y]->GetType();
				BYTE idNextType = pBrick[pos.x][pos.y]->GetNextType();
				if (idNextType == idType || idNextType == 0)
					break;
				RemoveBrick( pos );
				InsertBrick( idNextType, pos );
			}
		}
}


void CBrickArray::FallBricks() 
{
	for (int x=0; x<Max.x; x++)
		for (int y=Max.y-2; y>=0; y--)
			if (pBrick[x][y] && pBrick[x][y]->GetType() != pBrick[x][y]->GetNextType() && !pBrick[x][y+1] ) {
				pBrick[x][y+1] = pBrick[x][y];
				pBrick[x][y] = NULL;
				pBrick[x][y+1]->vPosition.y += pBrick[x][y+1]->vSize.y;
			}
}


void CBrickArray::ExpandExploding()
{
	// tymczasowa lista eksploduj¹cych cegie³ek
	list<POINT> listPos;
	PushExplosive( &listPos );

	// dla ka¿dej cegie³ki wybuchowej zmieñ przylegaj¹ce cegie³ki na wybuchowe
	list<POINT>::iterator iPos;
	for (iPos=listPos.begin(); iPos!=listPos.end(); iPos++) {
		POINT posAdj;
		for (posAdj.x = iPos->x-1; posAdj.x <= iPos->x+1; posAdj.x++)
			for (posAdj.y = iPos->y-1; posAdj.y <= iPos->y+1; posAdj.y++) {
				if (!IsValid( posAdj ))
					continue;
				CBrick* pBrick = GetBrick( posAdj );
				if (pBrick && !pBrick->IsExplosive()) {
					RemoveBrick( posAdj );
					InsertBrick( BRICK_TYPE_EXPL, posAdj );
				}
			}
	}
}


void CBrickArray::PushExplosive( list<POINT>* pList )
{
	POINT pos;
	for (pos.x=0; pos.x<Max.x; pos.x++)
		for (pos.y=0; pos.y<Max.y; pos.y++) {
			CBrick* pBrick = GetBrick( pos );
			if (pBrick && pBrick->IsExplosive()) {
				pList->push_back( pos );
			}
		}
}
