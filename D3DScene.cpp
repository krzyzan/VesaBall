#include "StdAfx.h"
#include "D3DScene.h"

LPDIRECT3DDEVICE8 CD3DScene::pD3DDevice;

CD3DScene::CD3DScene()
{
	pCurrentScene = this;
}


CD3DScene::~CD3DScene()
{
	// release the textures
	while (!stackTextures.empty()) {
		stackTextures.top()->Release();
		stackTextures.pop();
	}
}

CD3DScene* CD3DScene::GetNextScene()
{
	CD3DScene* pScene = pCurrentScene;
	pCurrentScene = this;

	return pScene;
}

HRESULT CD3DScene::LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex )
{
	D3DXCreateTextureFromFile( pD3DDevice, strFileName, pTex );
	stackTextures.push( *pTex );

	return S_OK;
}
