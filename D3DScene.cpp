#include "stdafx.h"
#include "d3dscene.h"

CD3DScene::CD3DScene( LPDIRECT3DDEVICE8 d3dDevice )
{
	pD3DDevice	= d3dDevice;
}


CD3DScene::~CD3DScene()
{
	// zwalniamy tekstury
	vector<LPDIRECT3DTEXTURE8>::iterator iTex;
	for (iTex = vctrTextures.begin(); iTex != vctrTextures.end(); iTex++)
		SAFE_RELEASE(*iTex);
}


HRESULT CD3DScene::LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex )
{
    D3DXCreateTextureFromFile( pD3DDevice, strFileName, pTex );
	vctrTextures.push_back( *pTex );

	return S_OK;
}
