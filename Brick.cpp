// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "Brick.h"
#include "Ball.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrick::CBrick( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition )
: CSprite( iTexture, D3DXVECTOR2(1.0f, 1.0f), 0, iPosition, 0xFFFFFFFF )
{

}

CBrick::~CBrick()
{

}

void CBrick::FrameMove( FLOAT fElapsedTime )
{

}

void CBrick::Collide( list<CBall*>* pListBall )
{
	list<CBall*>::iterator iBall;
	for (iBall = pListBall->begin(); iBall != pListBall->end(); iBall++)
		(*iBall)->Bounce( this );
};