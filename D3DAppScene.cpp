#include "stdafx.h"
#include "d3dappscene.h"

CD3DAppScene::CD3DAppScene( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
{
	pd3dDevice	= d3dDevice;
	pDIDevice	= DIDevice;
}


CD3DAppScene::~CD3DAppScene()
{
	// zwalniamy tekstury
	vector<LPDIRECT3DTEXTURE8>::iterator iTex;
	for (iTex = vtrTexture.begin(); iTex != vtrTexture.end(); iTex++)
		SAFE_RELEASE(*iTex);
}


HRESULT CD3DAppScene::LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex )
{
    D3DXCreateTextureFromFile( pd3dDevice, strFileName, pTex );
	// TODO: zrobiæ obs³uge b³êdów
	vtrTexture.push_back( *pTex );

	return S_OK;
}
