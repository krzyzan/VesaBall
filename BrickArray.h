#pragma once
#include "Brick.h"

class CBrickArray
{
public:
	static const LONG MAX_X = 20;
	static const LONG MAX_Y = 20;

	CBrickArray();
	virtual ~CBrickArray();

	void Render( LPD3DXSPRITE pSprite ) const;

	void ZapBricks();
	void FallBricks();

	bool Contains( const D3DXVECTOR2 & vPos ) const;
	POINT VectorToArrayCoords( const D3DXVECTOR2 & vPos ) const;
	
	void InsertBrick( BYTE type, const POINT & pos );

	HRESULT Load( char* strFileName );
	HRESULT Save( char* strFileName ) const;

	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;

	CBrick*	pBrick[MAX_X][MAX_Y];
};