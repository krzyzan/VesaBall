#ifndef BALL_H
#define BALL_H

#include "SpriteMoving.h"

const D3DXVECTOR2 BALL_SIZE_MIN = D3DXVECTOR2(1.0f / 128, 1.0f / 128);
const D3DXVECTOR2 BALL_SIZE_MAX = D3DXVECTOR2(1.0f / 56, 1.0f / 56);
const D3DXVECTOR2 BALL_SIZE_AVG = (BALL_SIZE_MIN + BALL_SIZE_MAX) / 2;

const float BALL_SPEED_VAL_MIN = 0.20f;
const float BALL_SPEED_VAL_MAX = 0.80f;
const float BALL_SPEED_VAL_AVG = 0.40f;

//! Ball
/*!
	\warning Before creating instances of this class, the texture must be loaded into #spTexture.
*/
class CBall : public CSpriteMoving
{
public:
	//! Constructor
	/*!
		Creates a ball of normal size using the #spTexture texture
		\param Position		Initial position
		\param Speed		Initial speed
	*/
	CBall(const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed);

	virtual ~CBall();

	//! Moves the ball
	/*!
		\copydoc CSpriteMoving::FrameMove(float fElapsedTime)
		If the ball is "caught" by the paddle, does nothing.
		\param fElapsedTime	Time since the last call
	*/
	void FrameMove(float fElapsedTime);

	//! Sets the speed to the nearest value within the <BALL_SPEED_MIN, BALL_SPEED_MAX> range
	/*!
		\param vNewSpeed	New speed.
	*/
	void SetSpeed(const D3DXVECTOR2 & vNewSpeed);

	//! Changes the direction of the speed vector when bouncing off a rectangular object
	/*!
		\param pSprite		Pointer to the object
		\param vSide		Vector of the collision point relative to the ball's center
	*/
	void Reflect(const CSprite* pSprite, const D3DXVECTOR2 & vSide);

	//! If \b true, the ball does not move; defaults to \b false
	bool bCatched;

	//! Address of the shared texture.
	/*!
		\warning Set before creating instances of this class.
	*/
	static LPDIRECT3DTEXTURE8 spTexture;
};

#endif
