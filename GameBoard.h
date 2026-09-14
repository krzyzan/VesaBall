#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Scene.h"
#include "BrickArray.h"

//! Game scenery
/*!
	Side boundaries and the brick array
*/
class CGameBoard : public CScene
{
protected:
	CGameBoard();
	virtual ~CGameBoard();

	//! Initializes the scene's objects and loads its textures.
	/*!
		Loads the needed textures into memory.
		Creates the side boundaries and loads the brick array from a file.
	*/
	HRESULT OnInit();

	//! Releases the scene's objects.
	HRESULT OnDestroy();

	//! Ends the scene if the user pressed the Escape key
	HRESULT OnKeyboardEvent(const InputEvent* evt);

	//! Renders the game scenery
	HRESULT FrameRender();

protected:
	list<CSprite*> listSprite;
	CBrickArray* pBrickArray;
	DWORD dwLevelNum;
};

#endif
