#pragma once

#include <list>
using namespace std;

#include "Ball.h"

class CLevel
{
public:
	CLevel();
	~CLevel();

	list<CSprite*>			listRender;
	list<CMovingSprite*>	listFrameMove;
	list<CBall*>			listBall;

	BOOL bThruBrick;
};
