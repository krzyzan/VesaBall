#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <list>
using namespace std;

#include "Scene.h"

class CMenuItem;
class CCursor;
class CSprite;

class CGameMenu : public CScene
{
public:
	enum UID_MSG
	{
		UID_START = 100,
		UID_EDITOR,
		UID_QUIT,
	};

	CGameMenu();
	virtual ~CGameMenu();

	HRESULT OnInit();
	HRESULT OnDestroy();

	HRESULT OnMouseEvent(const InputEvent* evt);
	HRESULT OnKeyboardEvent(const InputEvent* evt);

	HRESULT FrameMove(float fElapsedTime);
	HRESULT FrameRender();

private:
	CMenuItem* GetPOINTedMenuItem() const;

	list<CSprite*> listRender;
	list<CMenuItem*> listMenuItem;
	CCursor* pCursor;

	CMenuItem* pCurrentItem;
	CMenuItem* pPressedItem;
};

#endif
