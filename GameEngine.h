#pragma once

#include "GameBoard.h"

class CBall;
class CSpriteMoving;
class CSpriteEffect;
class CSpriteAnimated;
class CPaddle;
class CBonus;
class CCounter;

//! Engine gry VesaBall
class CGameEngine :
	public CGameBoard
{
public:
	CGameEngine();
	virtual ~CGameEngine();

	HRESULT InitDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod );
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();

private:
	void MoveObjects( float fElapsedTime );
	void CollideObjects();
	void CollideBallPaddle( CBall* pBall );
	void CollideBallBricks( CBall* pBall );
	void DeleteExpiredObjects( list<CSpriteMoving*> *pList );

	void ApplyBonus( DWORD Type );
	void BoardPrepare();
	void BoardClear();
	void BoardReset();
	void KillPaddle();
	
	void CreateSparkles( CBall* pBall, const D3DXVECTOR2 & vSide );
	void DestroyBrick( const POINT & pos);
	void DoExplosion( const D3DXVECTOR2 & vPosition );

private:
	list<CBall*>			listBall;
	list<CBonus*>			listBonus;
	list<CSpriteEffect*>	listEffect;
	list<CSpriteAnimated*>	listExplosion;
	CCounter*				pScoreCounter; 
	CCounter*				pLivesCounter; 
	CPaddle*				pPaddle;

	LPDIRECT3DTEXTURE8		pSparkTex;
	LPDIRECT3DTEXTURE8		pExplosionTex;

	bool					bThruBrick;
	bool					bFireBall;
	bool					bFallingBricks;

	bool					bPaused;

	float					fGameSpeed;				//TODO: TMP

	float					fTimeToBallTail;
	float					fTimeToBrickExplode;
	list<POINT>				listExploding;
};
