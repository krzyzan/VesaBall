#include "StdAfx.h"
#include "GameEditor.h"
#include "GameMenu.h"

#include "Counter.h"
#include "Cursor.h"
#include "Brick.h"

CGameEditor::CGameEditor()
{
	pCursor = NULL;
	curType = 1;
	pLevelCounter = 0;

	bLMB = false;
	bRMB = false;
}

CGameEditor::~CGameEditor()
{
}

HRESULT CGameEditor::OnInit()
{
	CGameBoard::OnInit();

	SDL_Texture* pTex;
	LoadTexture("gfx/Cursor_arrow.png", &pTex);
	pCursor = new CCursor(pTex);

	// create the level counter
	LoadTexture("gfx/Digits.png", &CCounter::spTexture);
	POINT DigitPixels = {64, 92};
	pLevelCounter = new CCounter(0, Vec2(BOARD_W * 0.2f, 0.05f * 0.75f), Vec2(BOARD_L + BOARD_W * 0.125f, 0.05f / 2), DigitPixels, 6);

	// create the brick legend
	POINT arraySize = {10, 4};
	pBrickToolkit = new CBrickArray(arraySize, Vec2(BOARD_L + BOARD_W / 2, BOARD_B - BOARD_W / 10), Vec2(BOARD_W / 2, BOARD_W / 10));

	POINT pos;
	BYTE idType = 0;
	for (pos.y = 0; pos.y < arraySize.y; pos.y++)
		for (pos.x = 0; pos.x < arraySize.x; pos.x++)
		{
			idType++;
			if (idType >= BRICK_TYPE_MAX)
				return S_OK;
			pBrickToolkit->InsertBrick(idType, pos);
		}

	return S_OK;
}

HRESULT CGameEditor::OnMouseEvent(const InputEvent* evt)
{
	switch (evt->ofs)
	{
	case InputEvent::AxisX:
		pCursor->Move(Vec2((float)evt->data, 0));
		break;

	case InputEvent::AxisY:
		pCursor->Move(Vec2(0, (float)evt->data));
		break;

	case InputEvent::Button0:
		if (evt->data & 0x80)
		{
			POINT pos = pBrickToolkit->GetArrayCoordsAt(pCursor->vPosition);
			if (pBrickToolkit->IsValid(pos))
			{
				CBrick* pBrick = pBrickToolkit->GetBrick(pos);
				if (pBrick)
					curType = pBrick->GetType();
			}
		}
		bLMB = ((evt->data & 0x80) != 0);
		break;

	case InputEvent::Button1:
		bRMB = ((evt->data & 0x80) != 0);
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CGameEditor::OnKeyboardEvent(const InputEvent* evt)
{
	if (evt->ofs == InputEvent::Key && (evt->data & 0x80))
		switch (evt->scancode)
		{
		case SDL_SCANCODE_RIGHT:
			if (dwLevelNum < NUM_LEVELS - 1)
			{
				dwLevelNum++;
				pLevelCounter->lValue = dwLevelNum;
				pBrickArray->Load(dwLevelNum);
			}
			return S_OK;

		case SDL_SCANCODE_LEFT:
			if (dwLevelNum > 0)
			{
				dwLevelNum--;
				pLevelCounter->lValue = dwLevelNum;
				pBrickArray->Load(dwLevelNum);
			}
			return S_OK;

		case SDL_SCANCODE_L:
			pBrickArray->Load(dwLevelNum);
			return S_OK;

		case SDL_SCANCODE_S:
			pBrickArray->Save(dwLevelNum);
			return S_OK;

		case SDL_SCANCODE_C:
			pBrickArray->Clear();
			return S_OK;

		default:
			break;
		}

	return CGameBoard::OnKeyboardEvent(evt);
}

HRESULT CGameEditor::FrameMove(float fElapsedTime)
{
	pLevelCounter->Update(fElapsedTime);

	if (bLMB || bRMB)
	{
		POINT pos = pBrickArray->GetArrayCoordsAt(pCursor->vPosition);
		if (pBrickArray->IsValid(pos))
		{
			pBrickArray->RemoveBrick(pos);
			if (bLMB)
				pBrickArray->InsertBrick(curType, pos);
		}
	}

	return S_OK;
}

HRESULT CGameEditor::FrameRender()
{
	SDL_SetRenderDrawColor(pRenderer, 0x40, 0x60, 0x60, 255);
	SDL_RenderClear(pRenderer);

	CGameBoard::FrameRender();

	pBrickToolkit->Render(pRenderer);
	pLevelCounter->Render(pRenderer);
	pCursor->Render(pRenderer);

	return S_OK;
}

HRESULT CGameEditor::OnDestroy()
{
	SAFE_DELETE(pCursor);
	SAFE_DELETE(pLevelCounter);
	delete pBrickToolkit;

	return CGameBoard::OnDestroy();
}
