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

	//! Initializes objects in system memory
	/*!
		\copydoc CGameBoard::OnInitDevice()
		Creates the cursor.
	*/
	HRESULT OnInitDevice();

	//! Releases objects in system memory
	HRESULT OnDeleteDevice();

	//! Processes mouse events
	/*!
		With the left button, the user creates bricks.
		With the right button, bricks are removed. The brick type can be selected
		by left-clicking on the bottom array.
	*/

	HRESULT OnMouseEvent(LPDIDEVICEOBJECTDATA didod);

	//! Processes keyboard events
	/*!
		- The Escape key exits without saving.
		- The Right key moves to the next level
		- The Left key moves to the previous level
		- The S key saves the level.
		- The L key loads the level.
		- The C key clears all bricks.
	*/
	HRESULT OnKeyboardEvent(LPDIDEVICEOBJECTDATA didod);

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
