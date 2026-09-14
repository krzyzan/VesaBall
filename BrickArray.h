#ifndef BRICK_ARRAY_H
#define BRICK_ARRAY_H

#include "Brick.h"

const DWORD NUM_LEVELS = 50;

//! Brick array
/*!
	\par
	Stores #CBrick objects. Allows finding the brick at a given screen point in constant time.
	Counts the bricks that can be "destroyed".
*/
class CBrickArray
{
public:
	//! Creates an empty brick array
	/*
		/param	Size		Number of columns and rows
		/param	Position	Position of the brick array's center
		/param	ScreenSize	On-screen size of the brick array
	*/
	CBrickArray(const POINT & Size, const Vec2 & Position, const Vec2 & ScreenSize);

	virtual ~CBrickArray();

	//! Removes all bricks from the array
	void Clear();

	//! Renders the brick array
	/*!
		\param pRenderer	the SDL renderer used for rendering
	*/
	void Render(SDL_Renderer* pRenderer) const;

	//! Inserts a brick into the array
	/*!
		\param	idType	Brick type
		\param	pos		Position in the array
		\warning If there is already a brick at this position, it must be removed first!
	*/
	void InsertBrick(BYTE idType, const POINT & pos);

	//! Removes a brick
	/*!
		If there is no brick at the given position, does nothing.
		\param	pos		Position in the array
	*/
	void RemoveBrick(const POINT & pos);

	//! Returns \b true if there are no more bricks left to destroy in the array
	bool Empty() const
	{
		return !dwBricksLeft;
	}

	//! Loads the brick array from a file
	/*!
		All bricks currently in the array are removed.
		\param	dwLevelNum	Level number
		\warning No error handling
	*/
	void Load(DWORD dwLevelNum);

	//! Saves the brick array to a file
	/*!
		\param	dwLevelNum	Level number
		\warning No error handling
	*/
	void Save(DWORD dwLevelNum) const;

	//! Returns \b true if the coordinates belong to the array
	/*!
		\param	pos	Coordinates
	*/
	bool IsValid(const POINT & pos) const
	{
		return (pos.x >= 0) && (pos.x < Max.x) && (pos.y >= 0) && (pos.y < Max.y);
	}

	//! Returns the array coordinates corresponding to a point on screen
	/*!
		\param	vPos	Point on screen
	*/
	POINT GetArrayCoordsAt(const Vec2 & vPos) const;

	//! Returns the on-screen point corresponding to array coordinates
	Vec2 GetPositionAt(const POINT & pos) const;

	//! Returns a pointer to the brick at the given position
	/*!
		\param	pos		Array coordinates
		\return	Pointer to the brick
	*/
	CBrick* GetBrick(const POINT & pos) const
	{
		return pBrick[pos.x][pos.y];
	}

	//! Reduces the durability of all bricks to 1
	void ZapBricks();

	//! Lowers the bricks
	/*!
		Lowers the bricks that have free space beneath them.
		Does not move "indestructible" bricks, which prevents the ball from getting stuck.
	*/
	void FallBricks();

	//! Bricks adjacent to explosive ones become explosive
	void ExpandExploding();

	//! Adds the positions of all explosive bricks to the list
	void PushExplosive(list<POINT>* pList);

private:
	Vec2 vSize;
	Vec2 vPosition;

	CBrick*** pBrick;
	POINT Max;

	DWORD dwBricksLeft;
};

#endif
