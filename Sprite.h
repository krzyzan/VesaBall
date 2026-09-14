#ifndef SPRITE_H
#define SPRITE_H

#include <d3dx8.h>

//! Sprite (textured 2D object)
/*!
	The CSprite class simplifies working with the \b ID3DXSprite interface.
	It makes rendering resolution-independent by using screen coordinates, 
	i.e. top-left \c (0.00f, 0.00f), bottom-right \c (1.00f, 0.75f)
	The position and rotation center are set at the sprite's center.
*/

class CSprite  
{
public:
		// Constructor
		/*!
			\param Texture	texture used for rendering
			\param Size		sprite size in screen coordinates
			\param Rotation	rotation angle
			\param Position	position of the center in screen coordinates
			\param Color	The color and alpha channel are multiplied by this value. The value \c 0xFFFFFFFF preserves the original source color.
		*/
	CSprite( const LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
			float Rotation, const D3DXVECTOR2 & Position, D3DCOLOR Color );
	virtual ~CSprite();

		//! Renders the sprite
		/*!
			\param pSprite The \b ID3DXSprite object used for rendering.
		*/
	virtual void Render( LPD3DXSPRITE pSprite ) const;

		//! Sets the sprite's size via scaling.
	void SetSize( const D3DXVECTOR2 & Size );

		//! Flips the sprite vertically
	void FlipV() 
		{ vScaling.x *= -1; }

		//! Flips the sprite horizontally
	void FlipH() 
		{ vScaling.y *= -1; }

		//! Changes the color
		/*!
			\param Color	The color and alpha channel are multiplied by this value. The value \c 0xFFFFFFFF preserves the original source color.
		*/
	void SetColor( D3DCOLOR Color ) 
		{ dwColor = Color; }

	D3DXVECTOR2			vPosition;
	D3DXVECTOR2			vSize;

protected:
	LPDIRECT3DTEXTURE8	pTexture;
	D3DXVECTOR2			vScaling;
	float				fRotation;
	D3DXVECTOR2			vRotationCenter;
	D3DCOLOR			dwColor;
};

#endif
