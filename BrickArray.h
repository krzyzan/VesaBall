#pragma once
#include "Brick.h"

class CNewBrick
{
public:
	static const DUR_MAX = 3;
	static const BYTE TYPE_MAX = 5;

	struct STypeDesc
	{
		BYTE				type;
		DWORD				dur;
		LPDIRECT3DTEXTURE8	pTexture[DUR_MAX];
	};

	static STypeDesc s_TypeDesc[TYPE_MAX];

/////
	CNewBrick();

	CBrick*	pBrick;
	STypeDesc* pTypeDesc;
	DWORD dwHitCounter;

	D3DXVECTOR2			vSize;
	D3DXVECTOR2			vPosition;
	D3DXVECTOR2			vScaling;		//TODO: protected

	void SetHitCounter( DWORD HitCounter );

	void SetSize( const D3DXVECTOR2 & Size );
};

inline CNewBrick::CNewBrick()
{
	dwHitCounter = 0;
}

inline void CNewBrick::SetHitCounter( DWORD HitCounter )
{
	if (pTypeDesc->dur != 0xFFFFFFFF) {
		dwHitCounter = HitCounter;
	}
}

inline void CNewBrick::SetSize( const D3DXVECTOR2 & Size )
{
	vSize			= Size;
	D3DSURFACE_DESC sd;
	pTypeDesc->pTexture[dwHitCounter]->GetLevelDesc(0, &sd);
	vScaling		= D3DXVECTOR2( vSize.x/sd.Width, vSize.y/sd.Height ) * RES_X;
}

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

	CNewBrick* GetBrickAt( const POINT & pos ){ return &NewBrick[pos.x][pos.y]; };
	void CreateBrick( const POINT & pos, BYTE type );

	HRESULT Load( char* strFileName );
	HRESULT Save( char* strFileName ) const;

	D3DXVECTOR2 vSize;
	D3DXVECTOR2 vPosition;

private:
	CNewBrick NewBrick[MAX_X][MAX_Y];
};