#include "StdAfx.h"
#include "GameMenu.h"
#include "MenuItem.h"
#include "Cursor.h"

#include "GameEngine.h"
#include "GameEditor.h"

CGameMenu::CGameMenu()
{
	pCursor = NULL;

	pPressedItem = NULL;
}

CGameMenu::~CGameMenu()
{
}

HRESULT CGameMenu::OnInit()
{
	SDL_Texture* pTex;

	CMenuItem* pMenuItem;
	LoadTexture("gfx/Menu_Title.png", &pTex);
	pMenuItem = new CMenuItem(pTex, Vec2(0.8f, 0.2f), Vec2(0.5f, 0.15f), 0xFFFFFFFF, 0);
	listRender.push_back(pMenuItem);

	LoadTexture("gfx/Menu_Start.png", &pTex);
	pMenuItem = new CMenuItem(pTex, Vec2(0.4f, 0.1f), Vec2(0.5f, 0.4f), 0xFFFFCC00, UID_START);
	listRender.push_back(pMenuItem);
	listMenuItem.push_back(pMenuItem);

	LoadTexture("gfx/Menu_Editor.png", &pTex);
	pMenuItem = new CMenuItem(pTex, Vec2(0.4f, 0.1f), Vec2(0.5f, 0.5f), 0xFFFFCC00, UID_EDITOR);
	listRender.push_back(pMenuItem);
	listMenuItem.push_back(pMenuItem);

	LoadTexture("gfx/Menu_Quit.png", &pTex);
	pMenuItem = new CMenuItem(pTex, Vec2(0.4f, 0.1f), Vec2(0.5f, 0.6f), 0xFFFFCC00, UID_QUIT);
	listRender.push_back(pMenuItem);
	listMenuItem.push_back(pMenuItem);

	LoadTexture("gfx/Cursor_arrow.png", &pTex);
	pCursor = new CCursor(pTex);
	listRender.push_back(pCursor);

	pCurrentItem = GetPOINTedMenuItem();
	if (pCurrentItem)
		pCurrentItem->SetHighlighted(true);

	return S_OK;
}

CMenuItem* CGameMenu::GetPOINTedMenuItem() const
{
	list<CMenuItem*>::const_iterator iMenuItem;
	for (iMenuItem = listMenuItem.begin(); iMenuItem != listMenuItem.end(); iMenuItem++)
		if (fabs((*iMenuItem)->vPosition.y - pCursor->vPosition.y) < (*iMenuItem)->vSize.y / 2 &&
			fabs((*iMenuItem)->vPosition.x - pCursor->vPosition.x) < (*iMenuItem)->vSize.x / 2)
			break;
	return (iMenuItem != listMenuItem.end()) ? *iMenuItem : NULL;
}

HRESULT CGameMenu::OnMouseEvent(const InputEvent* evt)
{
	switch (evt->ofs)
	{
	case InputEvent::AxisX:
		pCursor->Move(Vec2((float)evt->data, 0));
		break;
	case InputEvent::AxisY:
		pCursor->Move(Vec2(0, (float)evt->data));
		break;
	default:
		break;
	}

	CMenuItem* pOldMenuItem;

	switch (evt->ofs)
	{
	case InputEvent::AxisX:
	case InputEvent::AxisY:
		pOldMenuItem = pCurrentItem;
		pCurrentItem = GetPOINTedMenuItem();
		if (pPressedItem && pCurrentItem != pPressedItem)
			pCurrentItem = NULL;
		if (pCurrentItem != pOldMenuItem)
		{
			if (pOldMenuItem)
				pOldMenuItem->SetHighlighted(false);
			if (pCurrentItem)
				pCurrentItem->SetHighlighted(true);
		}
		break;

	case InputEvent::Button0:
		if (evt->data & 0x80)
		{ // button pressed
			if (pCurrentItem)
			{
				pPressedItem = pCurrentItem;
				pPressedItem->SetPressed(true);
			}
		}
		else
		{
			if (pPressedItem)
			{ // button released
				if (pCurrentItem == pPressedItem)
				{
					switch (pCurrentItem->GetUID())
					{
					case UID_START:
						SetCurrentScene(new CGameEngine());
						break;
					case UID_EDITOR:
						SetCurrentScene(new CGameEditor());
						break;
					case UID_QUIT:
						SetCurrentScene(NULL);
						break;
					}
				}
				pPressedItem->SetPressed(false);
				pPressedItem = NULL;
			}
		}
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CGameMenu::OnKeyboardEvent(const InputEvent* evt)
{
	if (evt->ofs == InputEvent::Key && evt->scancode == SDL_SCANCODE_ESCAPE && (evt->data & 0x80))
		SetCurrentScene(NULL);

	return S_OK;
}

HRESULT CGameMenu::FrameMove(float fElapsedTime)
{
	return S_OK;
}

HRESULT CGameMenu::FrameRender()
{
	SDL_SetRenderDrawColor(pRenderer, 58, 110, 145, 255);
	SDL_RenderClear(pRenderer);

	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render(pRenderer);

	return S_OK;
}

HRESULT CGameMenu::OnDestroy()
{
	// Remove from the render list
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}
