// Brick.h: interface for the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CBrick
{
public:
	static const DUR_MAX = 3;
	static const BYTE TYPE_MAX = 5;

	struct STypeDesc
	{
		DWORD				dur;
		LPDIRECT3DTEXTURE8	pTexture[DUR_MAX];
	};

	static STypeDesc s_TypeDesc[TYPE_MAX];

/////
	CBrick();

	STypeDesc* pTypeDesc;
	DWORD dwHitCounter;

	//D3DXVECTOR2			vPosition;

	void SetHitCounter( DWORD HitCounter );
};