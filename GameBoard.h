#pragma once
#include "D3DAppScene.h"
#include "Brick.h"

const int BRICK_X = 20;
const int BRICK_Y = 20;
const float BRICK_TABLE_T = 0.05f;
const float BRICK_TABLE_B = 0.55f;
const float BRICK_TABLE_H = BRICK_TABLE_B - BRICK_TABLE_T;


class CGameBoard :
	public CD3DAppScene
{
public:
	CGameBoard( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CGameBoard(void);

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT RenderLoop();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	void Reset();
	HRESULT LoadLevel( char* strFileName );
	HRESULT SaveLevel( char* strFileName );

protected:
	void InsertBrick( BYTE type, const POINT & pos );
	void RemoveBrick( const POINT & pos );

	list<CSprite*>	listRender;
	LPD3DXSPRITE	pSprite;
	
	CBrick*			pBrickTable[BRICK_X][BRICK_Y];

	LPDIRECT3DTEXTURE8		pWallTex;
	LPDIRECT3DTEXTURE8		pBrickTex[BRICK_TYPES];
};
