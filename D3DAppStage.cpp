#include "stdafx.h"
#include "d3dappstage.h"

CD3DAppStage::CD3DAppStage( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
{
	pd3dDevice	= d3dDevice;
	pDIDevice	= DIDevice;
}

CD3DAppStage::~CD3DAppStage()
{
}
