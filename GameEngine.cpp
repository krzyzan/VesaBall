#include "StdAfx.h"
#include "GameEngine.h"
#include "GameMenu.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Bonus.h"
#include "Counter.h"
#include "SpriteEffect.h"
#include "SpriteAnimated.h"

const DWORD INITIAL_LIVES = 2;	// Initial number of lives
const float BONUS_PROB = 0.05f; // Probability of generating a bonus
const DWORD MAX_BALLS = 32;		// Maximum number of balls
const DWORD MAX_EFFECTS = 64;	// Maximum number of effects

const float BALL_SHIFT = 0.01f;		// Initial ball offset on the paddle
const float FIREBALL_TAIL = 0.005f; // Time between spawns
const float EXPL_PROP_TIME = 0.15f; // Explosion propagation time
const float EXPL_BLOW = 0.1f;		// Explosion blast force coefficient
const float BALL_BLOW = 10.0f;		// Ball impact force coefficient
const float PADDLE_SPEED = 1.5f;	// Paddle sensitivity to mouse movement
const float GAME_SPEED = 1.0f;		// Game speed

const DWORD BONUS_SCORE = 100; // Number of points for catching a bonus

CGameEngine::CGameEngine()
{
	fGameSpeed = GAME_SPEED;
	bCheats = false;

	bPaused = false;

	pScoreCounter = NULL;
	pLivesCounter = NULL;
	pPaddle = NULL;

	fTimeToExplosion = 0;
	fTimeToBallTail = 0;
}

CGameEngine::~CGameEngine()
{
}

HRESULT CGameEngine::OnInit()
{
	CGameBoard::OnInit();

	// paddle
	LoadTexture("gfx/Paddle.png", &CPaddle::spTexture);
	LoadTexture("gfx/Lightning.png", &CPaddle::spLightningTexture);

	// ball
	LoadTexture("gfx/Ball_alu.png", &CBall::spTexture);
	LoadTexture("gfx/SparkEffect.png", &pSparkTex);

	// Explosions
	LoadTexture("gfx/Explosion.png", &pExplosionTex);
	// The explosion/fireball-tail animation uses a "brightening" blend
	// (replacing the original's SetRenderState(SRCBLEND=DESTCOLOR,
	// DESTBLEND=ONE) toggled around that render pass) instead of normal
	// alpha blending. SDL applies blend mode per-texture rather than as a
	// global render state, and pExplosionTex is only ever used for this
	// animation, so it's set once here instead of toggled every frame.
	SDL_SetTextureBlendMode(pExplosionTex,
							SDL_ComposeCustomBlendMode(
								SDL_BLENDFACTOR_DST_COLOR, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
								SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD));

	// load the bonus textures
	LoadTexture("gfx/Bonus_Thrubrick.PNG", &CBonus::spTextures[CBonus::ThruBrick]);
	LoadTexture("gfx/Bonus_SetOffExploding.PNG", &CBonus::spTextures[CBonus::SetOffExploding]);
	LoadTexture("gfx/Bonus_Fireball.PNG", &CBonus::spTextures[CBonus::FireBall]);
	LoadTexture("gfx/Bonus_ShootingPaddle.png", &CBonus::spTextures[CBonus::ShootingPaddle]);
	LoadTexture("gfx/Bonus_GrabPaddle.png", &CBonus::spTextures[CBonus::GrabPaddle]);

	LoadTexture("gfx/Bonus_ExtraLife.PNG", &CBonus::spTextures[CBonus::ExtraLife]);
	LoadTexture("gfx/Bonus_LevelWarp.png", &CBonus::spTextures[CBonus::LevelWarp]);
	LoadTexture("gfx/Bonus_ZapBricks.png", &CBonus::spTextures[CBonus::ZapBricks]);
	LoadTexture("gfx/Bonus_SlowBall.png", &CBonus::spTextures[CBonus::SlowBall]);
	LoadTexture("gfx/Bonus_ExpandExploding.png", &CBonus::spTextures[CBonus::ExpandExploding]);

	LoadTexture("gfx/Bonus_KillPaddle.png", &CBonus::spTextures[CBonus::KillPaddle]);
	LoadTexture("gfx/Bonus_ShrinkBall.png", &CBonus::spTextures[CBonus::ShrinkBall]);
	LoadTexture("gfx/Bonus_FastBall.png", &CBonus::spTextures[CBonus::FastBall]);
	LoadTexture("gfx/Bonus_SuperShrink.png", &CBonus::spTextures[CBonus::SuperShrink]);
	LoadTexture("gfx/Bonus_FallingBricks.png", &CBonus::spTextures[CBonus::FallingBricks]);

	LoadTexture("gfx/Bonus_ExpandPaddle.png", &CBonus::spTextures[CBonus::ExpandPaddle]);
	LoadTexture("gfx/Bonus_ShrinkPaddle.png", &CBonus::spTextures[CBonus::ShrinkPaddle]);
	LoadTexture("gfx/Bonus_SplitBall.png", &CBonus::spTextures[CBonus::SplitBall]);
	LoadTexture("gfx/Bonus_MegaBall.png", &CBonus::spTextures[CBonus::MegaBall]);
	LoadTexture("gfx/Bonus_EightBall.png", &CBonus::spTextures[CBonus::EightBall]);

	// create the counter
	LoadTexture("gfx/Digits.png", &CCounter::spTexture);
	POINT DigitPixels = {64, 92};
	pScoreCounter = new CCounter(0, Vec2(BOARD_W * 0.2f, 0.05f * 0.75f), Vec2(BOARD_L + BOARD_W * 0.125f, 0.05f / 2), DigitPixels, 6);
	pLivesCounter = new CCounter(INITIAL_LIVES, Vec2(BOARD_W * 0.2f, 0.05f * 0.75f), Vec2(BOARD_R - BOARD_W * 0.125f, 0.05f / 2), DigitPixels, 6);

	BoardReset();

	return S_OK;
}

HRESULT CGameEngine::OnDestroy()
{
	BoardClear();

	SAFE_DELETE(pScoreCounter);
	SAFE_DELETE(pLivesCounter);

	return CGameBoard::OnDestroy();
}

void CGameEngine::BoardPrepare()
{
	// create the paddle with a ball
	pPaddle = new CPaddle();
	CBall* pBall = new CBall(pPaddle->vPosition + Vec2(BALL_SHIFT, 0), Vec2(0, BALL_SPEED_VAL_AVG));
	listBall.push_front(pBall);
	pPaddle->CatchBall(pBall);
}

void CGameEngine::BoardClear()
{
	SAFE_DELETE(pPaddle);

	// Remove from the ball list
	while (!listBall.empty())
	{
		delete (*listBall.begin());
		listBall.pop_front();
	}

	// Remove from the effect list
	while (!listEffect.empty())
	{
		delete (*listEffect.begin());
		listEffect.pop_front();
	}

	// Remove from the explosion list
	while (!listExplosion.empty())
	{
		delete (*listExplosion.begin());
		listExplosion.pop_front();
	}

	// Remove from the bonus list
	while (!listBonus.empty())
	{
		delete (*listBonus.begin());
		listBonus.pop_front();
	}

	listExplodingPos.clear();

	bThruBrick = false;
	bFireBall = false;
	// TODO:	ShootingPaddle = false;
	bFallingBricks = false;
}

void CGameEngine::BoardReset()
{
	BoardClear();
	BoardPrepare();
}

HRESULT CGameEngine::OnMouseEvent(const InputEvent* evt)
{
	if (pPaddle && !bPaused)
		switch (evt->ofs)
		{
		case InputEvent::AxisX:
			pPaddle->Move((float)evt->data * PADDLE_SPEED / RES_X);
			break;

		case InputEvent::Button0:
			if (evt->data & 0x80) // button pressed
				pPaddle->LaunchCatchedBalls();
			break;

		default:
			break;
		}

	if (!bCheats)
		return S_OK;

	if (evt->ofs == InputEvent::Button1)
	{
		if (evt->data & 0x80) // button pressed
			fGameSpeed = GAME_SPEED / 5;
		else // button released
			fGameSpeed = GAME_SPEED;
	}

	return S_OK;
}

HRESULT CGameEngine::OnKeyboardEvent(const InputEvent* evt)
{
	if (evt->ofs == InputEvent::Key && (evt->data & 0x80))
		switch (evt->scancode)
		{
		case SDL_SCANCODE_P:
		case SDL_SCANCODE_PAUSE:
			bPaused = !bPaused;
			return S_OK;
		case SDL_SCANCODE_SYSREQ:
			bCheats = !bCheats;
			return S_OK;
		case SDL_SCANCODE_SPACE:
			if (bCheats)
				pBrickArray->Clear();
			return S_OK;
		case SDL_SCANCODE_B:
			if (bCheats)
			{
				int type = listBonus.empty() ? 0 : (listBonus.front()->GetType() + 1) % CBonus::MAX_TYPE;
				CBonus* pBonus = new CBonus(static_cast<CBonus::TypeEnum>(type), Vec2(0.50f, 0.375f), Vec2(0, 0));
				listBonus.push_front(pBonus);
			}
			return S_OK;
		default:
			break;
		}

	return CGameBoard::OnKeyboardEvent(evt);
}

HRESULT CGameEngine::FrameMove(float fElapsedTime)
{
	if (bPaused)
		return S_OK;

	MoveObjects(fElapsedTime * fGameSpeed);
	CollideObjects();

	// Remove from the lists
	DeleteExpiredObjects((list<CSpriteMoving*>*)&listEffect);
	DeleteExpiredObjects((list<CSpriteMoving*>*)&listExplosion);
	DeleteExpiredObjects((list<CSpriteMoving*>*)&listBonus);
	DeleteExpiredObjects((list<CSpriteMoving*>*)&listBall);

	// lose a life
	if (listBall.empty() || !pPaddle)
	{
		pLivesCounter->lValue--;

		// TODO: gameover :)
		if (pLivesCounter->lValue >= 0)
			BoardReset();
		else
			SetCurrentScene(NULL);
	}

	// advance to the next level
	if (pBrickArray->Empty())
	{
		dwLevelNum++;

		if (dwLevelNum >= NUM_LEVELS)
		{
			SetCurrentScene(NULL); // TODO: highscores :)
			return S_OK;
		}

		BoardReset();
		pBrickArray->Load(dwLevelNum);
	}

	return S_OK;
}

HRESULT CGameEngine::FrameRender()
{
	// render
	SDL_SetRenderDrawColor(pRenderer, 0x40, 0x60, 0x60, 255);
	SDL_RenderClear(pRenderer);

	// scenery
	CGameBoard::FrameRender();

	// vanishing bricks
	list<CSpriteEffect*>::iterator iEffect;
	for (iEffect = listEffect.begin(); iEffect != listEffect.end(); iEffect++)
		(*iEffect)->Render(pRenderer);

	// balls
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		(*iBall)->Render(pRenderer);

	// bonuses
	list<CBonus*>::iterator iBonus;
	for (iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++)
		(*iBonus)->Render(pRenderer);

	// paddle
	if (pPaddle)
		pPaddle->Render(pRenderer);

	// explosions
	list<CSpriteAnimated*>::iterator iExplosion;
	for (iExplosion = listExplosion.begin(); iExplosion != listExplosion.end(); iExplosion++)
		(*iExplosion)->Render(pRenderer);

	// counters
	pScoreCounter->Render(pRenderer);
	pLivesCounter->Render(pRenderer);

	return S_OK;
}

void CGameEngine::MoveObjects(float fElapsedTime)
{
	// exploding bricks
	fTimeToExplosion -= fElapsedTime * listExplodingPos.size();

	while (fTimeToExplosion < 0)
	{
		if (!listExplodingPos.empty())
		{
			POINT pos = listExplodingPos.front();
			listExplodingPos.pop_front();
			DoExplosion(pos);
		}
		fTimeToExplosion += EXPL_PROP_TIME;
	}

	// tail for the fireball
	if (bFireBall)
		fTimeToBallTail -= fElapsedTime;

	while (fTimeToBallTail < 0)
	{
		// balls
		list<CBall*>::iterator iBall;
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			CreateFireballTail(*iBall);
		fTimeToBallTail += FIREBALL_TAIL;
	}

	// move the balls
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		(*iBall)->FrameMove(fElapsedTime);

	// move the bonuses
	list<CBonus*>::iterator iBonus;
	for (iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++)
		(*iBonus)->FrameMove(fElapsedTime);

	// move the effects
	list<CSpriteEffect*>::iterator iEffect;
	for (iEffect = listEffect.begin(); iEffect != listEffect.end(); iEffect++)
		(*iEffect)->FrameMove(fElapsedTime);

	// move the explosions
	list<CSpriteAnimated*>::iterator iExplosion;
	for (iExplosion = listExplosion.begin(); iExplosion != listExplosion.end(); iExplosion++)
		(*iExplosion)->FrameMove(fElapsedTime);

	// update the counter
	pScoreCounter->Update(fElapsedTime);
	pLivesCounter->Update(fElapsedTime);
}

void CGameEngine::CollideObjects()
{
	// bounce the balls off the bricks
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		CollideBallBricks(*iBall);

	// if there is no paddle, stop here
	if (!pPaddle)
		return;

	// bounce the balls off the paddle
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		CollideBallPaddle(*iBall);

	// catch the bonuses
	list<CBonus*>::iterator iBonus = listBonus.begin();
	while (iBonus != listBonus.end())
	{
		if (!pPaddle)
			break;
		if ((*iBonus)->GetCollisionSide(pPaddle) != Vec2(0, 0))
		{
			ApplyBonus((*iBonus)->GetType());
			iBonus = listBonus.erase(iBonus);
			continue;
		}
		iBonus++;
	}
}

void CGameEngine::CollideBallPaddle(CBall* pBall)
{
	if (pBall->bCatched)
		return;

	Vec2 vSide = pBall->GetCollisionSide(pPaddle);
	if (vSide == Vec2(0, 0))
		return;

	if (pPaddle->bGrabPaddle && vSide.y && fabs(pBall->vPosition.x - pPaddle->vPosition.x) < pPaddle->vSize.x / 3)
		pPaddle->CatchBall(pBall);
	else
	{
		pBall->Reflect(pPaddle, vSide);
		CreateSparkles(pBall, vSide);
	}
	pPaddle->SetBallSpeed(pBall);

	if (bFallingBricks)
		pBrickArray->FallBricks();
}

void CGameEngine::CollideBallBricks(CBall* pBall)
{
	for (LONG x = 0; x < 2; x++)
		for (LONG y = 0; y < 2; y++)
		{
			Vec2 vPos = pBall->vPosition - pBall->vSize / 2 + Vec2(pBall->vSize.x * x, pBall->vSize.y * y);
			POINT pos = pBrickArray->GetArrayCoordsAt(vPos);
			if (!pBrickArray->IsValid(pos))
				continue;

			CBrick* pBrick = pBrickArray->GetBrick(pos);
			if (pBrick == NULL)
				continue;

			Vec2 vSide = pBall->GetCollisionSide(pBrick);

			if (!bThruBrick)
			{
				pBall->Reflect(pBrick, vSide);
				CreateSparkles(pBall, vSide);
			}

			if (bFireBall)
				DoExplosion(pos);
			else
			{
				BYTE idNextType = pBrick->GetNextType();
				if (idNextType && !bThruBrick)
				{
					listEffect.push_front(pBrick->CreateBlendEffect(Vec2(0, 0)));
					pBrickArray->RemoveBrick(pos);
					pBrickArray->InsertBrick(idNextType, pos);
				}
				else
					DestroyBrick(pos, vSide * BALL_BLOW);
			}
		}
}

void CGameEngine::DoExplosion(const POINT & pos)
{
	for (int i = 0; i < 2; i++)
	{
		POINT ptFramePixels = {64, 64};
		CSpriteAnimated* pAnimation = new CSpriteAnimated(pExplosionTex, Vec2(1.0f, 1.0f) / 8, frand(0, PI * 2),
														  pBrickArray->GetPositionAt(pos) + Vec2(frand(-0.01f, 0.01f), frand(-0.01f, 0.01f)), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0xFFFFFFFF, 1.0f, 0, 44, ptFramePixels);
		listExplosion.push_back(pAnimation);
	}

	POINT posAdj;
	for (posAdj.x = pos.x - 1; posAdj.x <= pos.x + 1; posAdj.x++)
		for (posAdj.y = pos.y - 1; posAdj.y <= pos.y + 1; posAdj.y++)
			if (pBrickArray->IsValid(posAdj))
				DestroyBrick(posAdj, Vec2(float(posAdj.x - pos.x), float(posAdj.y - pos.y)) * EXPL_BLOW);
}

void CGameEngine::DestroyBrick(const POINT & pos, const Vec2 & vSpeed)
{
	CBrick* pBrick = pBrickArray->GetBrick(pos);
	if (!pBrick)
		return;

	if (pBrick->IsExplosive())
		listExplodingPos.push_back(pos);

	listEffect.push_front(pBrick->CreateBlendEffect(vSpeed));
	pScoreCounter->lValue += pBrick->GetScore();

	if (frand(0, 1) < BONUS_PROB)
	{
		CBonus* pBonus = new CBonus(static_cast<CBonus::TypeEnum>(rand() % CBonus::MAX_TYPE), pBrick->vPosition, vSpeed);
		listBonus.push_back(pBonus);
	}

	pBrickArray->RemoveBrick(pos);
}

void CGameEngine::ApplyBonus(DWORD Type)
{
	pScoreCounter->lValue += BONUS_SCORE;

	list<CBall*>::iterator iBall;

	switch (Type)
	{
	case CBonus::ThruBrick:
		bThruBrick = true;
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			(*iBall)->SetColor(0xFF7FAFFF);
		break;

	case CBonus::SetOffExploding:
		pBrickArray->PushExplosive(&listExplodingPos);
		break;

	case CBonus::FireBall:
		bFireBall = true;
		if (bThruBrick)
			break;

		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			(*iBall)->SetColor(0xFFFFFF7F);
		break;

		/*
		TODO: ShootingPaddle,
		*/

	case CBonus::GrabPaddle:
		pPaddle->bGrabPaddle = true;
		break;

		//////

	case CBonus::ExtraLife:
		pLivesCounter->lValue++;
		break;

	case CBonus::LevelWarp:
		pBrickArray->Clear();
		break;

	case CBonus::ZapBricks:
		pBrickArray->ZapBricks();
		break;

	case CBonus::SlowBall:
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		{
			Vec2 vSpeed = (*iBall)->GetSpeed();
			Vec2Normalize(&vSpeed, &vSpeed);
			(*iBall)->SetSpeed(vSpeed * BALL_SPEED_VAL_MIN);
		}
		break;

	case CBonus::ExpandExploding:
		pBrickArray->ExpandExploding();
		break;

		//////

	case CBonus::KillPaddle:
		KillPaddle();
		break;

	case CBonus::ShrinkBall:
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			(*iBall)->SetSize(BALL_SIZE_MIN);
		break;

	case CBonus::FastBall:
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		{
			Vec2 vSpeed = (*iBall)->GetSpeed();
			Vec2Normalize(&vSpeed, &vSpeed);
			(*iBall)->SetSpeed(vSpeed * BALL_SPEED_VAL_MAX);
		}
		break;

	case CBonus::SuperShrink:
		pPaddle->SetWidth(PADDLE_WIDTH_MIN);
		break;

	case CBonus::FallingBricks:
		bFallingBricks = true;
		break;

		//////

	case CBonus::ExpandPaddle:
		pPaddle->SetWidth(pPaddle->vSize.x * 2);
		break;

	case CBonus::ShrinkPaddle:
		pPaddle->SetWidth(pPaddle->vSize.x / 2);
		break;

	case CBonus::SplitBall:
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			if (!(*iBall)->bCatched)
			{
				if (listBall.size() > MAX_BALLS)
					continue;
				CBall* pBall = new CBall(**iBall);
				Vec2 vSpeed = (*iBall)->GetSpeed();
				pBall->SetSpeed(Vec2(vSpeed.y, -vSpeed.x));
				listBall.push_front(pBall);
			}
		break;

	case CBonus::MegaBall:
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			(*iBall)->SetSize(BALL_SIZE_MAX);
		break;

	case CBonus::EightBall:
	{
		ApplyBonus(CBonus::FastBall);
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			if (!(*iBall)->bCatched)
			{
				CBall* pBall = *iBall;
				for (int i = 1; i < 8; i++)
				{
					if (listBall.size() > MAX_BALLS)
						continue;
					pBall = new CBall(*pBall);
					Vec2 vNewSpeed = Vec2Rotate(pBall->GetSpeed(), PI / 4);
					pBall->SetSpeed(vNewSpeed);
					listBall.push_front(pBall);
				}
			}
		break;
	}
	}
}

void CGameEngine::CreateSparkles(CBall* pBall, const Vec2 & vSide)
{
	Vec2 vSparkSize = Vec2(1.0f / 256, 1.0f / 256);
	Vec2 vSparkPosition = pBall->vPosition + vSide;
	Vec2 vSparkGravity = Vec2(0, GRAV_ACCEL);
	for (int i = 0; i < 8; i++)
	{
		if (listEffect.size() >= MAX_EFFECTS)
			continue;

		float fSparkDuration = frand(0.5f, 1.0f);
		Vec2 vSparkSpeed = Vec2(frand(-1.0f, 1.0f), frand(-1.0f, 1.0f)) / 20 + pBall->GetSpeed() / 4;
		CSpriteEffect* pEffectSprite = new CSpriteEffect(pSparkTex, vSparkSize,
														 vSparkPosition, vSparkSpeed, vSparkGravity, 0xFFFFFFFF, fSparkDuration);
		listEffect.push_back(pEffectSprite);
	}
}

void CGameEngine::CreateFireballTail(CBall* pBall)
{
	POINT ptFramePixels = {64, 64};
	CSpriteAnimated* pAnimation = new CSpriteAnimated(pExplosionTex, pBall->vSize * 2, frand(0, PI * 2),
													  pBall->vPosition + Vec2(frand(-0.5f, 0.5f) * pBall->vSize.x, frand(-0.5f, 0.5f) * pBall->vSize.y),
													  Vec2(0, 0), Vec2(0, 0), bThruBrick ? 0xFF0000FF : 0xFFFFFFFF, frand(0.05f, 0.2f), 8, 44, ptFramePixels);
	listExplosion.push_back(pAnimation);
}

void CGameEngine::DeleteExpiredObjects(list<CSpriteMoving*>* pList)
{
	list<CSpriteMoving*>::iterator iObject;
	iObject = pList->begin();
	while (iObject != pList->end())
	{
		if ((*iObject)->HasExpired())
		{
			delete (*iObject);
			iObject = pList->erase(iObject);
		}
		else
			iObject++;
	}
}

void CGameEngine::KillPaddle()
{
	pPaddle->LaunchCatchedBalls();
	SAFE_DELETE(pPaddle);
}
