#include "StdAfx.h"
#include "Scene.h"

#include <SDL_image.h>

SDL_Renderer* CScene::pRenderer;

CScene::CScene()
{
	pCurrentScene = this;
}

CScene::~CScene()
{
	// release the textures
	while (!stackTextures.empty())
	{
		SDL_DestroyTexture(stackTextures.top());
		stackTextures.pop();
	}
}

CScene* CScene::GetNextScene()
{
	CScene* pScene = pCurrentScene;
	pCurrentScene = this;

	return pScene;
}

HRESULT CScene::LoadTexture(const char* strFileName, SDL_Texture** pTex)
{
	*pTex = IMG_LoadTexture(pRenderer, strFileName);
	if (!*pTex)
		return E_FAIL;

	stackTextures.push(*pTex);

	return S_OK;
}
