#ifndef COUNTER_H
#define COUNTER_H

#include "Sprite.h"

//! Counter displaying positive numbers
/*!
	Before creating instances of this class, the texture must be loaded
	into #spTexture. The texture should contain the digits 0123456789
	arranged vertically.
*/
class CCounter : public CSprite
{
public:
	//! Constructor
	/*!
		\param Value		Initial value
		\param Size			Size
		\param Position		Position
		\param DigitPixels	Digit size within the texture, in pixels
		\param NumDigits	Number of digits displayed
	*/
	CCounter(LONG Value, const D3DXVECTOR2 & Size, const D3DXVECTOR2 & Position,
			 POINT DigitPixels, DWORD NumDigits);

	virtual ~CCounter();

	//! Renders the counter
	/*!
		\param pSprite	address of the \e ID3DXSprite used for rendering
	*/
	void Render(LPD3DXSPRITE pSprite) const;

	//! Updates the displayed number
	/*!
		\param fElapsedTime Time since the last call
	*/
	void Update(float fElapsedTime);

	//! Address of the shared texture.
	/*!
		\warning Set before creating instances of this class.
	*/
	static LPDIRECT3DTEXTURE8 spTexture;
	LONG lValue;

protected:
	POINT dwDigitPixels;
	DWORD dwNumDigits;
	float fValue;
};

#endif
