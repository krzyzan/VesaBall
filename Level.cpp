#include "stdafx.h"
#include "level.h"

CLevel::CLevel(void)
{  
	bThruBrick = FALSE;
}

CLevel::~CLevel(void)
{
	// Kasujemy z listy renderowania
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);
}
