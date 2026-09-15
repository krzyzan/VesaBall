#include "StdAfx.h"
#include "App.h"

CApp::CApp(const char* WindowTitle)
{
	bRunning = false;
	pWindow = NULL;
	pRenderer = NULL;
	strWindowTitle = WindowTitle;
}

HRESULT CApp::Create()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return E_FAIL;

	pWindow = SDL_CreateWindow(strWindowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   RES_X, RES_Y, SDL_WINDOW_SHOWN);
	if (!pWindow)
		return E_FAIL;

	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!pRenderer)
		return E_FAIL;

	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);

	CScene::pRenderer = pRenderer;

	// The game draws its own cursor sprite, so hide the OS cursor and read
	// the mouse as a stream of relative-motion deltas -- the same shape as
	// the buffered DirectInput mouse events the game logic already expects.
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);

	srand((DWORD)Timer.GetAbsoluteTime());
	Timer.Start();

	bRunning = true;

	return S_OK;
}

HRESULT CApp::StartNewScene(CScene* pScene)
{
	HRESULT hr;

	// Pushing a child scene only suspends the current one -- it stays alive
	// on the stack (and keeps its sprites/textures) so it can resume exactly
	// where it left off if/when the child ends and StartParentScene() pops
	// back to it. OnDestroy() tears a scene down for good, so it must only
	// run once, in StartParentScene(), when the scene is actually leaving
	// the stack -- calling it here too would delete this scene's sprites
	// while it's merely paused, leaving the dangling pointers in e.g.
	// CGameMenu::listRender to be dereferenced the next time it's rendered.
	sScenes.push(pScene);

	if (FAILED(hr = sScenes.top()->OnInit()))
		return hr;

	return S_OK;
}

HRESULT CApp::StartParentScene()
{
	HRESULT hr;

	if (FAILED(hr = sScenes.top()->OnDestroy()))
		return hr;

	delete sScenes.top();
	sScenes.pop();

	if (sScenes.empty())
	{
		bRunning = false;
		return S_OK;
	}

	return S_OK;
}

HRESULT CApp::ChangeScene()
{
	CScene* pCurrentScene = sScenes.top()->GetNextScene();

	if (pCurrentScene == NULL)
		return StartParentScene();

	if (pCurrentScene != sScenes.top())
		return StartNewScene(pCurrentScene);

	return S_OK;
}

void CApp::ProcessEvent(const SDL_Event & event)
{
	if (sScenes.empty())
		return;

	InputEvent evt;

	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		if (event.motion.xrel != 0)
		{
			evt.ofs = InputEvent::AxisX;
			evt.data = event.motion.xrel;
			sScenes.top()->OnMouseEvent(&evt);
		}
		if (event.motion.yrel != 0)
		{
			evt.ofs = InputEvent::AxisY;
			evt.data = event.motion.yrel;
			sScenes.top()->OnMouseEvent(&evt);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT)
			evt.ofs = InputEvent::Button0;
		else if (event.button.button == SDL_BUTTON_RIGHT)
			evt.ofs = InputEvent::Button1;
		else
			break;
		evt.data = (event.type == SDL_MOUSEBUTTONDOWN) ? 0x80 : 0;
		sScenes.top()->OnMouseEvent(&evt);
		break;

	case SDL_KEYDOWN:
	case SDL_KEYUP:
		if (event.key.repeat)
			break;
		evt.ofs = InputEvent::Key;
		evt.scancode = event.key.keysym.scancode;
		evt.data = (event.type == SDL_KEYDOWN) ? 0x80 : 0;
		sScenes.top()->OnKeyboardEvent(&evt);
		break;

	case SDL_QUIT:
		bRunning = false;
		break;
	}
}

HRESULT CApp::Run()
{
	SDL_Event event;

	while (bRunning)
	{
		while (SDL_PollEvent(&event))
			ProcessEvent(event);

		if (!bRunning || sScenes.empty())
			break;

		float fElapsedTime = Timer.GetElapsedTime();

		if (FAILED(sScenes.top()->FrameMove(fElapsedTime)))
			break;

		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(pRenderer);

		if (FAILED(sScenes.top()->FrameRender()))
			break;

		SDL_RenderPresent(pRenderer);

		if (FAILED(ChangeScene()))
			break;
	}

	while (!sScenes.empty())
	{
		sScenes.top()->OnDestroy();
		delete sScenes.top();
		sScenes.pop();
	}

	if (pRenderer)
		SDL_DestroyRenderer(pRenderer);
	if (pWindow)
		SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return S_OK;
}
