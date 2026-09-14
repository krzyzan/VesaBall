#ifndef BRICK_H
#define BRICK_H

#include "Sprite.h"

class CBall;
class CSpriteEffect;

const BYTE BRICK_TYPE_MAX = 23;
const BYTE BRICK_TYPE_EXPL = 8;

//! Brick
/*!
	Before use, textures must be loaded into the #spTexture array.
*/
class CBrick : public CSprite
{
	struct SType
	{
		DWORD dwScore;
		BYTE idNextType;
		bool bMustHit;
	};

public:
	//! Constructor
	/*!
		\param Type		Brick type
		\param Position	Initial position
		\param Size		Size
	*/
	CBrick(BYTE Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size);

	virtual ~CBrick();

	//! Creates (allocates) a vanishing-brick effect
	/*!
		\return The allocated #CSpriteEffect object
	*/
	CSpriteEffect* CreateBlendEffect(const D3DXVECTOR2 & vSpeed) const;

	//! Returns the brick's type index
	BYTE GetType() const
	{
		return idType;
	}

	//! Returns the brick's type index after being hit
	BYTE GetNextType() const
	{
		return s_Type[idType].idNextType;
	}

	//! Returns the score awarded for hitting the brick
	DWORD GetScore() const
	{
		return s_Type[idType].dwScore;
	}

	//! Returns \b true if the brick must be destroyed to clear the level
	bool MustHit() const
	{
		return s_Type[idType].bMustHit;
	}

	//! Returns \b true if explosive
	bool IsExplosive() const
	{
		return idType == BRICK_TYPE_EXPL;
	}

	//! Texture array
	/*!
		Static array of texture pointers, one for each brick type.
		\warning Load the textures before creating instances of this class.
	*/
	static LPDIRECT3DTEXTURE8 spTexture[BRICK_TYPE_MAX];

private:
	BYTE idType;

	static SType s_Type[BRICK_TYPE_MAX];
};

#endif
