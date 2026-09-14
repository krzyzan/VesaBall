#ifndef BONUS_H
#define BONUS_H

#include "SpriteMoving.h"

	//! Bonus
	/*!
		Before use, textures must be loaded into the #spTexture array.
	*/
class CBonus :
	public CSpriteMoving
{
public:
	//! Bonus types
	enum TypeEnum
	{
		ThruBrick,		//!< The ball passes through bricks instead of bouncing off them.
		SetOffExploding,//!< Detonates explosive bricks
		FireBall,		//!< Balls explode on hitting bricks
		ShootingPaddle,	//!< The paddle can shoot
		GrabPaddle,		//!< The paddle catches balls

		ExtraLife,		//!< Adds one life
		LevelWarp,		//!< Warps to the next level
		ZapBricks,		//!< Damages bricks that require multiple hits
		SlowBall,		//!< Slows down the balls
		ExpandExploding,//!< Bricks adjacent to explosive bricks also become explosive
	
		KillPaddle,		//!< Loses a life
		ShrinkBall,		//!< Shrinks the balls
		FastBall,		//!< Speeds up the balls
		SuperShrink,	//!< Shrinks the paddle to the minimum
		FallingBricks,	//!< Every bounce of the ball off the paddle makes the bricks descend

		ExpandPaddle,	//!< Widens the paddle
		ShrinkPaddle,	//!< Shrinks the paddle to the minimum
		SplitBall,		//!< Creates, for each ball, a new ball moving perpendicular to it
		MegaBall,		//!< Enlarges the balls
		EightBall,		//!< Every ball turns into eight balls.

		MAX_TYPE		//!< Reserved, used to determine the number of bonus types
	};

public:
		//! Constructor
		/*!
			Creates a falling bonus. Retrieves its texture from the #spTextures array based on type.
			\param Type		Bonus type
			\param Position	Initial position
			\param Speed	Initial speed
		*/ 
	CBonus( TypeEnum Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );

	virtual ~CBonus();
		
		//! Returns the bonus type
	TypeEnum GetType() const 
		{ return eType; }

		//! Texture array
		/*!
			Static array of texture pointers, one for each bonus type.
			\warning Load the textures before creating instances of this class.
		*/
	static LPDIRECT3DTEXTURE8 spTextures[MAX_TYPE];

private:
	TypeEnum eType;
};

#endif
