#pragma once
#include "Brick.h"

///////////////////////////////////////////////

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

	CBrick* GetBrickAt( const POINT & pos ){ return &NewBrick[pos.x][pos.y]; };
	void CreateBrick( const POINT & pos, LONG type );

	HRESULT Load( char* strFileName );
	HRESULT Save( char* strFileName ) const;

	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;

	D3DXVECTOR2			vBrickSize;
	D3DXVECTOR2			vBrickScaling;		//TODO: protected

private:
	CBrick NewBrick[MAX_X][MAX_Y];
};