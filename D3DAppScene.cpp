#include "stdafx.h"
#include "d3dappscene.h"

CD3DAppScene::CD3DAppScene( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
{
	pd3dDevice	= d3dDevice;
	pDIDevice	= DIDevice;
}

CD3DAppScene::~CD3DAppScene()
{
}
