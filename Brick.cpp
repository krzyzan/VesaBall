// Brick.cpp: implementation of the CBrick class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Brick.h"


CBrick::CBrick()
{
	dwHitCounter = 0;
}

void CBrick::SetHitCounter( DWORD HitCounter )
{
	if (pTypeDesc->dur != 0xFFFFFFFF) {
		dwHitCounter = HitCounter;
	}
}
