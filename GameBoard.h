#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "D3DScene.h"
#include "BrickArray.h"

	//! Game scenery
	/*!
		Side boundaries and the brick array
	*/
class CGameBoard :
	public CD3DScene
{
protected:
	CGameBoard();
	virtual ~CGameBoard();

		//! Initializes objects in system memory
		/*!
			Loads the needed textures into memory. 
			Creates the side boundaries and loads the brick array from a file.
		*/
	HRESULT OnInitDevice();

		//! Initializes objects in graphics card memory
		/*!
			Creates the \b ID3DXSprite object used for rendering
		*/
	HRESULT OnRestoreDevice();

		//! Releases objects in graphics card memory
	HRESULT OnInvalidateDevice();

		//! Releases objects in system memory
	HRESULT OnDeleteDevice();

		//! Ends the scene if the user pressed the Escape key
	HRESULT OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod );

		//! Renders the game scenery
	HRESULT FrameRender();

protected:
	LPD3DXSPRITE		pSprite;
	
	list<CSprite*>		listSprite;
	CBrickArray*		pBrickArray;
	DWORD				dwLevelNum;
};

#endif
