#ifndef GAMEEDITOR_H
#define GAMEEDITOR_H

#include "GameBoard.h"

class CCursor;
class CCounter;

//! Level editor
class CGameEditor : public CGameBoard
{
public:
	CGameEditor();
	virtual ~CGameEditor();

	//! Initializes the scene's objects and loads its textures.
	/*!
		\copydoc CGameBoard::OnInit()
		Creates the cursor.
	*/
	HRESULT OnInit();

	//! Releases the scene's objects.
	HRESULT OnDestroy();

	//! Processes mouse events
	/*!
		With the left button, the user creates bricks.
		With the right button, bricks are removed. The brick type can be selected
		by left-clicking on the bottom array.
	*/

	HRESULT OnMouseEvent(const InputEvent* evt);

	//! Processes keyboard events
	/*!
		- The Escape key exits without saving.
		- The Right key moves to the next level
		- The Left key moves to the previous level
		- The S key saves the level.
		- The L key loads the level.
		- The C key clears all bricks.
	*/
	HRESULT OnKeyboardEvent(const InputEvent* evt);

	HRESULT FrameMove(float fElapsedTime);
	HRESULT FrameRender();

protected:
	CCursor* pCursor;
	BYTE curType;
	CCounter* pLevelCounter;

	CBrickArray* pBrickToolkit;

	// mouse button state
	bool bLMB;
	bool bRMB;
};

#endif
