#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <list>
using namespace std;

#include "D3DScene.h"

class CMenuItem;
class CCursor;
class CSprite;

class CGameMenu : public CD3DScene
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

	HRESULT OnInitDevice();
	HRESULT OnRestoreDevice();
	HRESULT OnInvalidateDevice();
	HRESULT OnDeleteDevice();

	HRESULT OnMouseEvent(LPDIDEVICEOBJECTDATA didod);
	HRESULT OnKeyboardEvent(LPDIDEVICEOBJECTDATA didod);

	HRESULT FrameMove(float fElapsedTime);
	HRESULT FrameRender();

private:
	CMenuItem* GetPOINTedMenuItem() const;

	LPD3DXSPRITE pSprite;

	list<CSprite*> listRender;
	list<CMenuItem*> listMenuItem;
	CCursor* pCursor;

	CMenuItem* pCurrentItem;
	CMenuItem* pPressedItem;
};

#endif
