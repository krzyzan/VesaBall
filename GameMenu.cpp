#include "StdAfx.h"
#include "GameMenu.h"
#include "MenuItem.h"
#include "Cursor.h"

#include "GameEngine.h"
#include "GameEditor.h"

CGameMenu::CGameMenu()
{
	pSprite = NULL;
	pCursor = NULL;

	pPressedItem = NULL;
}

CGameMenu::~CGameMenu()
{
}

HRESULT CGameMenu::OnInitDevice()
{
	LPDIRECT3DTEXTURE8 pTex;

	CMenuItem* pMenuItem;
	LoadTexture("gfx/Menu_Title.png", &pTex);
	pMenuItem = new CMenuItem(pTex, D3DXVECTOR2(0.8f, 0.2f), D3DXVECTOR2(0.5f, 0.15f), 0xFFFFFFFF, 0);
	listRender.push_back(pMenuItem);

	LoadTexture("gfx/Menu_Start.png", &pTex);
	pMenuItem = new CMenuItem(pTex, D3DXVECTOR2(0.4f, 0.1f), D3DXVECTOR2(0.5f, 0.4f), 0xFFFFCC00, UID_START);
	listRender.push_back(pMenuItem);
	listMenuItem.push_back(pMenuItem);

	LoadTexture("gfx/Menu_Editor.png", &pTex);
	pMenuItem = new CMenuItem(pTex, D3DXVECTOR2(0.4f, 0.1f), D3DXVECTOR2(0.5f, 0.5f), 0xFFFFCC00, UID_EDITOR);
	listRender.push_back(pMenuItem);
	listMenuItem.push_back(pMenuItem);

	LoadTexture("gfx/Menu_Quit.png", &pTex);
	pMenuItem = new CMenuItem(pTex, D3DXVECTOR2(0.4f, 0.1f), D3DXVECTOR2(0.5f, 0.6f), 0xFFFFCC00, UID_QUIT);
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

HRESULT CGameMenu::OnRestoreDevice()
{
	D3DXCreateSprite(pD3DDevice, &pSprite);

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

HRESULT CGameMenu::OnMouseEvent(LPDIDEVICEOBJECTDATA didod)
{
	switch (didod->dwOfs)
	{
	case DIMOFS_X:
		pCursor->Move(D3DXVECTOR2((float)(int)didod->dwData, 0));
		break;
	case DIMOFS_Y:
		pCursor->Move(D3DXVECTOR2(0, (float)(int)didod->dwData));
		break;
	}

	CMenuItem* pOldMenuItem;

	switch (didod->dwOfs)
	{
	case DIMOFS_X:
	case DIMOFS_Y:
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

	case DIMOFS_BUTTON0:
		if (didod->dwData & 0x80)
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
	}

	return S_OK;
}

HRESULT CGameMenu::OnKeyboardEvent(LPDIDEVICEOBJECTDATA didod)
{
	if (didod->dwOfs == DIK_ESCAPE && (didod->dwData & 0x80))
		SetCurrentScene(NULL);

	return S_OK;
}

HRESULT CGameMenu::FrameMove(float fElapsedTime)
{
	return S_OK;
}

HRESULT CGameMenu::FrameRender()
{
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(58, 110, 145), 1.0f, 0);

	pSprite->Begin();
	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render(pSprite);
	pSprite->End();

	return S_OK;
}

HRESULT CGameMenu::OnInvalidateDevice()
{
	SAFE_RELEASE(pSprite);

	return S_OK;
}

HRESULT CGameMenu::OnDeleteDevice()
{
	// Remove from the render list
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}
