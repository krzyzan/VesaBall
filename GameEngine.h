#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GameBoard.h"

class CBall;
class CSpriteMoving;
class CSpriteEffect;
class CSpriteAnimated;
class CPaddle;
class CBonus;
class CCounter;

//! VesaBall game engine
class CGameEngine : public CGameBoard
{
public:
	CGameEngine();
	virtual ~CGameEngine();

	//! Initializes objects in system memory
	/*!
		\copydoc CGameBoard::OnInitDevice()
		\par
		Loads textures:
		- paddle
		- balls
		- bonuses
		- counters
		- effects
		\par
		Creates:
		- score counter
		- lives counter
		- paddle with a ball
	*/
	HRESULT OnInitDevice();

	//! Releases objects in system memory
	/*!
		\copydoc CGameBoard::OnDeleteDevice()
		\par
		Releases:
		- the paddle
		- balls
		- effects
		- bonuses
		- explosion positions
	*/
	HRESULT OnDeleteDevice();

	//! Processes events from the mouse buffer
	/*!
		\par
		Moves the paddle. The left button releases all balls "caught" by the paddle.
	*/
	HRESULT OnMouseEvent(LPDIDEVICEOBJECTDATA didod);

	//! Processes events from the keyboard buffer
	/*!
		\par
		The P key pauses the game.
	*/
	HRESULT OnKeyboardEvent(LPDIDEVICEOBJECTDATA didod);

	//! Moves the scene's objects and computes collisions
	/*!
		\par
		Moves the scene's objects and computes collisions.
		Removes objects that have "finished".
		If there are no more bricks left to collect, advances to the next level
		If all balls have fallen, uses up a life.
		If there are no lives left, exits the game.
		If the game is paused, does nothing.
		\param fElapsedTime	Time elapsed since the last call to this function
	*/
	HRESULT FrameMove(float fElapsedTime);

	//! Renders the scene
	HRESULT FrameRender();

private:
	//! Moves the scene's objects
	void MoveObjects(float fElapsedTime);

	//! Computes collisions between objects
	void CollideObjects();

	//! Computes the collision between a ball and the paddle
	/*!
		\par
		If the bonus has been collected, the paddle "catches" balls.
		Otherwise it bounces them off. The bounce angle increases toward the edges of the paddle.
		\param pBall	The ball
	*/
	void CollideBallPaddle(CBall* pBall);

	//! Computes collisions between a ball and the bricks
	/*!
		\par
		Destroys the brick the ball hits.
		Creates sparks on impact.
		If #CBonus::FireBall has been collected, creates an explosion.
		If #CBonus::FireBall has \b not been collected, bounces the ball off the bricks.
	*/
	void CollideBallBricks(CBall* pBall);

	//! Prepares the game to start
	void BoardPrepare();

	//! Clears the objects
	void BoardClear();

	//! Creates the paddle and the ball
	void BoardReset();

	//! Destroys the paddle, releasing all caught balls
	void KillPaddle();

	//! An explosion that "destroys" the bricks around it.
	/*!
		\param pos	Position of the brick in the array
	*/
	void DoExplosion(const POINT & pos);

	//! A hit on a brick
	/*!
		\par
		Removes the brick from the brick array, creating a vanishing-brick effect.
		Adds points for "destroying" the brick.
		A bonus may appear with a certain probability.
		If the brick is explosive, creates an explosion.
		\param pos		Position of the brick in the array
		\param vSpeed	Speed of the "destruction" and of the bonus created
	*/
	void DestroyBrick(const POINT & pos, const D3DXVECTOR2 & vSpeed);

	//! Activates the effects of a collected bonus
	/*!
		\param Type Type of the collected bonus
		\sa CBonus::TypeEnum
	*/
	void ApplyBonus(DWORD Type);

	//! Creates a spark effect when the ball bounces
	void CreateSparkles(CBall* pBall, const D3DXVECTOR2 & vSide);

	//! Creates the fireball's "tail"
	void CreateFireballTail(CBall* pBall);

	//! Removes moving objects that have finished
	/*!
		\param pList Pointer to the list of objects to remove from
		\sa CMovingSprite::HasExpired()
	*/
	void DeleteExpiredObjects(list<CSpriteMoving*>* pList);

private:
	list<CBall*> listBall;
	list<CBonus*> listBonus;
	list<CSpriteEffect*> listEffect;
	list<CSpriteAnimated*> listExplosion;
	CCounter* pScoreCounter;
	CCounter* pLivesCounter;
	CPaddle* pPaddle;

	LPDIRECT3DTEXTURE8 pSparkTex;
	LPDIRECT3DTEXTURE8 pExplosionTex;

	bool bThruBrick;
	bool bFireBall;
	bool bFallingBricks;

	bool bPaused;

	float fGameSpeed;
	float bCheats;

	float fTimeToBallTail;
	float fTimeToExplosion;
	list<POINT> listExplodingPos;
};

#endif
