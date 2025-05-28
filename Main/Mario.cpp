#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "ParaGoomba.h"
#include "VenusFire.h"
#include "GreenVenusFire.h"
#include "PiranhaTrap.h"
#include "Fire.h"
#include "Coin.h"
#include "Portal.h"
#include "ItemBox.h"
#include "SwitchBlock.h"
#include "Giant.h"
#include "Leaf.h"
#include "Card.h"
#include "Pipe.h"
#include "Collision.h"
#include "PlayScene.h"
#include "AddScoreEffect.h"


CMario* CMario::__instance = nullptr;

CMario::CMario(float x, float y) :CGameObject(x, y)
{
	sensorFr = new CTailSensor(x, y, 1);
	sensorFr ->SetOwner(this);
	sensorB = new CTailSensor(x, y, 0);
	sensorB->SetOwner(this);
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene)
	{
		scene->AddObject(sensorFr); // hoặc push vào vector<objects> tùy bạn tổ chức
		scene->AddObject(sensorB); // hoặc push vào vector<objects> tùy bạn tổ chức
	}
	__instance = this; // Gán instance
	isSitting = false;
	maxVx = 0.0f;
	ax = 0.0f;
	ay = MARIO_GRAVITY;
	LPGAMEOBJECT ea = new CEnemyActivator();
	ea->SetPosition(x + 100.0f, y);
	if (scene)
	{
		scene->AddObject(ea);
	}
	level = MARIO_LEVEL_TANUKI;
	untouchable = 0;
	untouchable_start = -1;
	kick_start = -1;
	isOnPlatform = false;
	endMapTargetX = 2870.0f;
	this->renderLayer = 10;
	if (CGame::GetInstance()->GetIsExitingPipe() == true)
	{
		SetState(MARIO_STATE_EXIT_PIPE);
	}
}
void CMario::StartFlap()
{
	if (!isFlying) // Nếu chưa bay, bật cờ bay lên
		isFlying = true;

	isFlapping = true;
	flap_start = GetTickCount64();
	if (isOnPlatform) vy = -TANUKI_JUMP_RUN_SPEED_Y;
	else vy = -TANUKI_FLAP_SPEED_Y;
}
void CMario::TakeDmg()
{
	if (untouchable == 0)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			SetLevel(MARIO_LEVEL_SMALL);
		}
		else if (level == MARIO_LEVEL_TANUKI)
		{
			SetLevel(MARIO_LEVEL_BIG);
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	DebugOutTitle(L"x: %f", x);
	if (atEndMap)
	{
		vx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		vy += ay * dt;
		if (x >= endMapTargetX)
		{
			vx = 0;
			return;
		}
		CCollision::GetInstance()->Process(this, dt, coObjects);
		// Không xử lý bất kỳ logic cập nhật nào khác nữa
		return;
	}
	CGame* game = CGame::GetInstance();
	
	if (state == MARIO_STATE_EXIT_PIPE)
	{
		const float ENTER_PIPE_SPEED = 0.025f; // Tốc độ chui xuống (đơn vị pixel/miligiây)
		y -= dt * ENTER_PIPE_SPEED; 
		if (y < 132.0f) // Khi lên đủ cao
		{
			pipeEnterX = 0.0f;
			pipeEnterY = 0.0f;
			enterPipeStart = 0;
			SetIsOnHiddenPipe(false);
			SetState(MARIO_STATE_IDLE); // Reset trạng thái
		}
	}

	if (state == MARIO_STATE_ENTER_PIPE)
	{	
		const float ENTER_PIPE_SPEED = 0.025f; // Tốc độ chui xuống (đơn vị pixel/miligiây)
		if (pipeEnterX != 0.0f && pipeEnterY != 0.0f)
		{
			int currentScene = CGame::GetInstance()->GetCurrentSceneId();
			if(currentScene == 3)
			{
				y += dt * ENTER_PIPE_SPEED; // Di chuyển xuống dần theo thời gian
				if (y > pipeEnterY + 25.0f) // Khi xuống đủ xa
				{
					pipeEnterX = 0.0f;
					pipeEnterY = 0.0f;
					enterPipeStart = 0;
					SetIsOnHiddenPipe(false);
					SetState(MARIO_STATE_IDLE); // Reset trạng thái
				}
			}
			else
			{
				y -= dt * ENTER_PIPE_SPEED; // Di chuyển xuống dần theo thời gian
				if (y < pipeEnterY - 40.0f) // Khi lên đủ cao
				{
					pipeEnterX = 0.0f;
					pipeEnterY = 0.0f;
					enterPipeStart = 0;
					SetIsOnHiddenPipe(false);
					SetState(MARIO_STATE_IDLE); // Reset trạng thái
				}
			}
			
		}
	}
	if (isTransforming)
	{
		if (GetTickCount64() - transform_start >= MARIO_TIME_RUNTOFLY)
		{
			isTransforming = false;
			finishTransforming = true;
			SetLevel(transform_to);
		}
		return;
	}
	
	bool isRunningFast = abs(vx) == MARIO_RUNNING_SPEED;

	// Nếu đang chạy nhanh và đứng trên nền
	if (isRunningFast && isOnPlatform)
	{
		if (runPower < MARIO_MAX_RUN_POWER)
		{
			if (running_start == 0)
				running_start = GetTickCount64();

			ULONGLONG elapsed = GetTickCount64() - running_start;
			if (elapsed >= MARIO_TIME_PER_RUN_POWER)
			{
				runPower++;
				running_start = GetTickCount64();
			}
		}

		if (runPower == MARIO_MAX_RUN_POWER)
		{
			if (!maxPower)
			{
				maxPower = true;
				if (level == MARIO_LEVEL_TANUKI)
					running_maxpower_start = GetTickCount64();
			}
		}
	}
	else
	{
		if (level == MARIO_LEVEL_TANUKI && maxPower)
		{
			// giữ maxPower trong 5s
			if (GetTickCount64() - running_maxpower_start >= MARIO_TIME_FLYTOWALK)
			{
				maxPower = false;
				runPower = 0;
				running_maxpower_start = 0;
				running_start = 0;
			}
		}
		else
		{
			// KHÔNG giảm power nếu đang trên không và vẫn chạy nhanh
			if (!isOnPlatform && isRunningFast)
			{
				// giữ nguyên runPower và maxPower
			}
			else
			{
				// chỉ giảm nếu không còn chạy nhanh
				if (runPower > 0)
				{
					if (running_start == 0)
						running_start = GetTickCount64();

					ULONGLONG elapsed = GetTickCount64() - running_start;
					if (elapsed >= MARIO_TIME_PER_RUN_POWER)
					{
						runPower--;
						running_start = GetTickCount64();
					}
				}
				else
				{
					running_start = 0;
				}

				maxPower = false;
				running_maxpower_start = 0;
			}
		}
	}
	

	if (isHolding)
	{
		if (!heldKoopa->GetIsBeingHeld() || heldKoopa -> GetState() == KOOPA_STATE_DIEBYSHELL)
		{
			isHolding = false;
			heldKoopa = nullptr;
		}	
		if (heldKoopa != nullptr)
		{
			if (!game->IsKeyDown(DIK_A))
			{
				// Thả Koopa ra
				isHolding = false;
				heldKoopa->SetIsBeingHeld(false);

				// Koopa chuyển sang trạng thái ACTIVE
				heldKoopa->SetState(KOOPA_STATE_ACTIVATE);

				// Đá theo hướng Mario đang nhìn
				if (nx > 0)
				{
					SetState(MARIO_STATE_KICK_RIGHT);
					heldKoopa->SetSpeed(KOOPA_ACTIVATE_SPEED, 0); // Koopa bay phải
				}
				else
				{
					SetState(MARIO_STATE_KICK_LEFT);
					heldKoopa->SetSpeed(-KOOPA_ACTIVATE_SPEED, 0); // Koopa bay trái
				}
				heldKoopa = nullptr;
			}
		}
	}

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (kick_start > 0 && GetTickCount64() - kick_start > MARIO_KICK_DURATION)
	{
		kick_start = 0;
	}
	if (tailattack_start > 0 && GetTickCount64() - tailattack_start > TANUKI_TAILATTACK_DURATION)
	{
		tailattack_start = 0;
	}
	if (addScoreStart > 0 && GetTickCount64() - addScoreStart > MARIO_ADDSCORETIME)
	{
		addScoreStart = 0;
		scaleScore = 1;
	}
	if (isFlapping && GetTickCount64() - flap_start > 100)
	{
		isFlapping = false;
	}
	if (isFloating && GetTickCount64() - floating_start > 100)
	{
		isFloating = false;
	}
	
	if (isOnPlatform)
	{
		isFlying = false;
		isFloating = false;
		if (level == MARIO_LEVEL_TANUKI) ay = TANUKI_GRAVITY;
	}
	if (state != MARIO_STATE_ENTER_PIPE)
	{
		vy += ay * dt;
		vx += ax * dt;
	}
	if (abs(vx) > abs(maxVx)) vx = maxVx;
	if (nx == 1 && ax < 0 && vx < 0) vx = 0;
	if (nx == -1 && ax > 0 && vx > 0) vx = 0;
	if (vx < 0 && x < 17) x = 17;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);

	if (dynamic_cast<CPipe*>(e->obj))
	{
		OnCollisionWithPipe(e);
		return;
	}
		
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CParaGoomba*>(e->obj))
		OnCollisionWithParaGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CItemBox*>(e->obj))
		OnCollisionWithItemBox(e);
	else if (dynamic_cast<CGiant*>(e->obj))
		OnCollisonWithGiant(e);
	else if (dynamic_cast<CCard*>(e->obj))
		OnCollisionWithCard(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CVenusFire*>(e->obj) || dynamic_cast<CFire*>(e->obj)
			|| dynamic_cast<CGreenVenusFire*>(e->obj) || dynamic_cast<CPiranhaTrap*>(e->obj))
		OnCollisionWithDmgObject(e);
	else if (dynamic_cast<CSwitchBlock*>(e->obj))
		OnCollisionWithSwitchBlock(e);
	
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa->GetIsBeingHeld() || koopa->GetState() == KOOPA_STATE_DIEBYSHELL) return;
	if (e->ny < 0)
	{
		int koopaState = koopa->GetState();
		int l = koopa->GetLevel();

		if (l == KOOPA_LEVEL_RED || l == KOOPA_LEVEL_GREEN)
		{
			switch (koopaState)
			{
				case KOOPA_STATE_WALKING:
					koopa->SetState(KOOPA_STATE_SHELL);
					break;
				case KOOPA_STATE_ACTIVATE:
					koopa->SetState(KOOPA_STATE_SHELL);
					break;
				case KOOPA_STATE_SHELL:
					koopa->SetState(KOOPA_STATE_ACTIVATE);
					break;
			}
		}
		else if (l == KOOPA_LEVEL_PARA)
		{
			if (koopaState == KOOPA_STATE_WALKING)
				koopa->SetLevel(KOOPA_LEVEL_GREEN);
		}
		float accY = GetAy();
		float jumpForce = MARIO_JUMP_DEFLECT_SPEED;
		if (accY == MARIO_GRAVITY) jumpForce /= 1;
		else if (accY == TANUKI_GRAVITY) jumpForce /= 2;
		else jumpForce = 0;
		vy = -jumpForce;
		if (addScoreStart > 0) scaleScore *= 2;
		if (scaleScore >= 8) scaleScore = 8;
		CHUD::GetInstance()->SetScore(100 * scaleScore);
		LPGAMEOBJECT effect = nullptr;
		effect = new CAddScoreEffect(x, y - 25, 100 * scaleScore);
		if (effect)
		{
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (scene)
			{
				scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
			}
		}
		if(addScoreStart == 0) addScoreStart = GetTickCount64();
		
	}
	else if (e->nx != 0) // Va chạm từ bên trái/phải
	{
		if (koopa->GetState() == KOOPA_STATE_SHELL && !koopa->GetIsBeingHeld())
		{
			if (!isHolding && (this->state == MARIO_STATE_RUNNING_LEFT || this->state == MARIO_STATE_RUNNING_RIGHT))
			{
				// Mario cầm Koopa
				heldKoopa = koopa;
				isHolding = true;
				koopa->SetIsBeingHeld(true); // Thêm hàm này trong Koopa
			}
			else
			{
				koopa->SetState(KOOPA_STATE_ACTIVATE);

				// Mario sẽ đá Koopa theo hướng phù hợp
				if (e->nx > 0)
				{// Koopa bên trái => Mario đá sang trái
					SetState(MARIO_STATE_KICK_LEFT);
					koopa->SetSpeed(-KOOPA_ACTIVATE_SPEED, 0); // Koopa bay trái
				}
				else
				{// Koopa bên phải => Mario đá sang phải
					SetState(MARIO_STATE_KICK_RIGHT);
					koopa->SetSpeed(KOOPA_ACTIVATE_SPEED, 0); // Koopa bay phải
				}
			}
		}
		else
		{
			if (untouchable == 0)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					SetLevel(MARIO_LEVEL_SMALL);
				}
				else if (level == MARIO_LEVEL_TANUKI)
				{
					SetLevel(MARIO_LEVEL_BIG);
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() == GOOMBA_STATE_DIEBYSHELL) return;
	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			float accY = GetAy();
			float jumpForce = MARIO_JUMP_DEFLECT_SPEED;
			if (accY == MARIO_GRAVITY) jumpForce /= 1;
			else if (accY == TANUKI_GRAVITY) jumpForce /= 2;
			else jumpForce = 0;
			vy = -jumpForce;
			CHUD::GetInstance()->SetScore(100);
			LPGAMEOBJECT effect = nullptr;
			effect = new CAddScoreEffect(x, y - 25, 100);
			if (effect)
			{
				CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (scene)
				{
					scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
				}
			}
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					SetLevel(MARIO_LEVEL_SMALL);
				}
				else if (level == MARIO_LEVEL_TANUKI)
				{
					SetLevel(MARIO_LEVEL_BIG);
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (goomba->GetState() == PARAGOOMBA_STATE_DIEBYSHELL) return;
	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() == PARAGOOMBA_STATE_DIE) return;
		if (goomba->GetState() == PARAGOOMBA_STATE_NORMAL_WALKING)
		{
			goomba->SetState(PARAGOOMBA_STATE_DIE);
		}
		else
		{
			goomba->SetState(PARAGOOMBA_STATE_NORMAL_WALKING);
		}
		float accY = GetAy();
		float jumpForce = MARIO_JUMP_DEFLECT_SPEED;
		if (accY == MARIO_GRAVITY) jumpForce /= 1;
		else if (accY == TANUKI_GRAVITY) jumpForce /= 2;
		else jumpForce = 0;
		vy = -jumpForce;
		if (addScoreStart > 0) scaleScore *= 2;
		if (scaleScore >= 8) scaleScore = 8;
		CHUD::GetInstance()->SetScore(100 * scaleScore);
		LPGAMEOBJECT effect = nullptr;
		effect = new CAddScoreEffect(x, y - 25, 100 * scaleScore);
		if (effect)
		{
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (scene)
			{
				scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
			}
		}
		if (addScoreStart == 0) addScoreStart = GetTickCount64();
		
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != PARAGOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					SetLevel(MARIO_LEVEL_SMALL);
				}
				else if (level == MARIO_LEVEL_TANUKI)
				{
					SetLevel(MARIO_LEVEL_BIG);
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithItemBox(LPCOLLISIONEVENT e) {
	CItemBox* itb = dynamic_cast<CItemBox*>(e->obj);
	if (!itb) return;

	if (itb->GetState() == ITEMBOX_STATE_IDLE && e->ny > 0) {
		// Truyền hướng Mario chạm vào (trái/phải)
		float marioX = x;
		float itemboxX = itb->GetX();

		int direction = marioX < itemboxX ? 1 : -1; // 1 là từ trái, -1 là từ phải
		itb->SetBounceDirection(direction); // Thêm hàm này
		itb->SetState(ITEMBOX_STATE_BOUNCING);
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	CHUD::GetInstance()->SetCoin(1);
	CHUD::GetInstance()->SetScore(50);
}

void CMario::OnCollisionWithCard(LPCOLLISIONEVENT e)
{
	CCard* card = dynamic_cast<CCard*>(e->obj);
	if (!card) return;

	if (!card->IsActivated())
	{
		card->Activate(); // Dừng đổi card và nhảy lên
		CardType type = card->GetCardType();
		DebugOutTitle(L"id: %d", (int)type);
		CHUD::GetInstance()->SetCard((int)type);
		atEndMap = true;
	}
}

void CMario::OnCollisonWithGiant(LPCOLLISIONEVENT e)
{
	CGiant* g = dynamic_cast<CGiant*>(e->obj);
	if (g->GetState() == GIANT_STATE_ACTIVATE)
	{
		CHUD::GetInstance()->SetScore(1000);
		LPGAMEOBJECT effect = nullptr;
		if (!g->GetIsGreenGiant()) 
		{
			SetLevel(MARIO_LEVEL_BIG);
			effect = new CAddScoreEffect(x, y - 20, 1000);
			if (effect)
			{
				CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (scene)
				{
					scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
				}
			}
		}
		else
		{
			CHUD::GetInstance()->SetLife(1);
			effect = new CAddScoreEffect(x, y - 20, 9000);
			if (effect)
			{
				CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (scene)
				{
					scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
				}
			}
		}
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* l = dynamic_cast<CLeaf*>(e->obj);
	e->obj->Delete();
	SetLevel(MARIO_LEVEL_TANUKI);
	CHUD::GetInstance()->SetScore(1000);
	LPGAMEOBJECT effect = nullptr;
	effect = new CAddScoreEffect(x, y - 20, 1000);
	if (effect)
	{
		CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (scene)
		{
			scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
		}
	}
}

void CMario::OnCollisionWithDmgObject(LPCOLLISIONEVENT e) {
	TakeDmg();
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	if (e -> ny < 0)
	{
		CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
	}
}
void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	CPipe* p = dynamic_cast<CPipe*>(e->obj);
	if (!p || !p->GetIsHiddenPipe()) return;

	int currentScene = CGame::GetInstance()->GetCurrentSceneId();
	if ((currentScene == 3 && e->ny < 0) && isOnPlatform)
	{
		if (x < 2312.0f && x > 2306.0f)
		{
			// Lưu vị trí pipe để xử lý sau
			float pipeX, pipeY;
			p->GetPosition(pipeX, pipeY);
			this->pipeEnterX = pipeX; // Thêm biến pipeEnterX để lưu vị trí
			this->pipeEnterY = y; // Thêm biến pipeEnterY

			SetIsOnHiddenPipe(true);
		}
	}
	else if (currentScene == 1 && e->ny > 0)
	{
		if (x < 358.0f && x > 352.0f)
		{
			// Lưu vị trí pipe để xử lý sau
			float pipeX, pipeY;
			p->GetPosition(pipeX, pipeY);
			this->pipeEnterX = pipeX; // Thêm biến pipeEnterX để lưu vị trí
			this->pipeEnterY = y; // Thêm biến pipeEnterY
			/*DebugOutTitle(L"y: %f", pipeEnterY);*/
			SetIsOnHiddenPipe(true);
		}
	}
}
//
// Get animation ID for small Mario
//
void CMario::OnCollisionWithSwitchBlock(LPCOLLISIONEVENT e)
{
	CSwitchBlock* sw = dynamic_cast<CSwitchBlock*>(e->obj);
	if (e->ny < 0)
	{
		if (sw->GetState() == SWITCHBLOCK_STATE_IDLE)
			sw->SetState(SWITCHBLOCK_STATE_ACTIVATED);
	}
}

int CMario::GetAniIdSmall()
{
	int aniId = -1;
	// Ưu tiên xét state KICK trước
	if (state == MARIO_STATE_KICK_RIGHT)
		return ID_ANI_MARIO_SMALL_KICK_RIGHT;
	else if (state == MARIO_STATE_KICK_LEFT)
		return ID_ANI_MARIO_SMALL_KICK_LEFT;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (!isHolding)
			{
				if (maxPower)
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
					else
						aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
				}
				else
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
					else
						aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
				}
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_SMALL_RUNSHELLRIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_RUNSHELLLEFT;
			}
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (!isHolding)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLESHELLRIGHT;
					else aniId = ID_ANI_MARIO_SMALL_IDLESHELLLEFT;
				}
			}
			else if (vx > 0)
			{
				if (ax < 0 && (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_RUNNING_LEFT))
				{
					if (!isHolding) aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_RUNSHELLLEFT;
				}
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)
					{
						if(!maxPower) aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
						else aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT_MAXPOWER;
					}
					else aniId = ID_ANI_MARIO_SMALL_RUNSHELLRIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X || ax == -MARIO_DECEL_WALK_X)
				{
					if (!isHolding) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_RUNSHELLRIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_RUNNING_RIGHT)) // Quay đầu khi thay đổi hướng
				{
					if (!isHolding) aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
					else aniId = ID_ANI_MARIO_SMALL_RUNSHELLRIGHT;
				}
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)
					{
						if (!maxPower) aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
						else aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT_MAXPOWER;
					}
					else aniId = ID_ANI_MARIO_SMALL_RUNSHELLLEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X || ax == MARIO_DECEL_WALK_X)
				{
					if (!isHolding) aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
					else aniId = ID_ANI_MARIO_SMALL_RUNSHELLLEFT;
				}
			}
	if (aniId == -1)
	{
		if (nx > 0)
		{
			if (!isHolding) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_IDLESHELLRIGHT;
		}
		else
		{
			if (!isHolding) aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			else aniId = ID_ANI_MARIO_SMALL_IDLESHELLLEFT;
		}
	}
	if (kick_start > 0)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
	}
	if (state == MARIO_STATE_ENTER_PIPE || state == MARIO_STATE_EXIT_PIPE)
		aniId = ID_ANI_MARIO_SMALL_STAND;
	return aniId;
}

int CMario::GetAniIdBig()
{
	int aniId = -1;
	// Ưu tiên anim đá nếu đang đá
	
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (!isHolding)
			{
				if (maxPower)
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
					else
						aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
				}
				else
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
					else
						aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
				}
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_RUNSHELLRIGHT;
				else
					aniId = ID_ANI_MARIO_RUNSHELLLEFT;
			}
			
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (!isHolding)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_IDLE_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLESHELLRIGHT;
					else aniId = ID_ANI_MARIO_IDLESHELLLEFT;
				}
			}
			else if (vx > 0)
			{
				if (ax < 0 && (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_RUNNING_LEFT))
				{
					if (!isHolding)	aniId = ID_ANI_MARIO_BRACE_RIGHT;
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLLEFT;
				}
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)
					{
						if(!maxPower) aniId = ID_ANI_MARIO_RUNNING_RIGHT;
						else aniId = ID_ANI_MARIO_RUNNING_RIGHT_MAXPOWER;
					}
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLRIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X || ax == -MARIO_DECEL_WALK_X)
				{
					if (!isHolding) aniId = ID_ANI_MARIO_WALKING_RIGHT;
					else aniId = ID_ANI_MARIO_RUNSHELLRIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_RUNNING_RIGHT))
				{
					if (!isHolding)	aniId = ID_ANI_MARIO_BRACE_LEFT;
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLRIGHT;
				}
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)
					{
						if (!maxPower) aniId = ID_ANI_MARIO_RUNNING_LEFT;
						else aniId = ID_ANI_MARIO_RUNNING_LEFT_MAXPOWER;
					}
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLLEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X || ax == MARIO_DECEL_WALK_X)
				{
					if(!isHolding) aniId = ID_ANI_MARIO_WALKING_LEFT;
					else aniId = ID_ANI_MARIO_RUNSHELLLEFT;
				}
			}

	if (aniId == -1)
	{
		if (nx > 0)
		{
			if (!isHolding) aniId = ID_ANI_MARIO_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_IDLESHELLRIGHT;
		}
		else
		{
			if (!isHolding) aniId = ID_ANI_MARIO_IDLE_LEFT;
			else aniId = ID_ANI_MARIO_IDLESHELLLEFT;
		}
	}

	if (kick_start > 0)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_KICK_LEFT;
	}
	if (state == MARIO_STATE_ENTER_PIPE || state == MARIO_STATE_EXIT_PIPE)
		aniId = ID_ANI_MARIO_STAND;
	return aniId;
}

int CMario::GetAniIdTanuki() {
	int aniId = -1;
	// Ưu tiên anim đá nếu đang đá

	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (!isHolding)
			{
				if (isFlying)
				{
					if (nx >= 0)
					{
						aniId = isFlapping ? ID_ANI_TANUKI_JUMP_RUN_RIGHT_DOWN : ID_ANI_TANUKI_JUMP_RUN_RIGHT_UP;
					}
					else
					{
						aniId = isFlapping ? ID_ANI_TANUKI_JUMP_RUN_LEFT_DOWN : ID_ANI_TANUKI_JUMP_RUN_LEFT_UP;
					}
				}
				else
				{
					if (isFloating)
					{
						if (nx >= 0)
						{
							aniId = isFlapping ? ID_ANI_TANUKI_JUMP_WALK_RIGHT_DOWN : ID_ANI_TANUKI_JUMP_WALK_RIGHT_UP;
						}
						else
						{
							aniId = isFlapping ? ID_ANI_TANUKI_JUMP_WALK_LEFT_DOWN : ID_ANI_TANUKI_JUMP_WALK_LEFT_UP;
						}
					}
					else
					{
						if (!maxPower)
						{
							if (nx >= 0)
								aniId = ID_ANI_TANUKI_JUMP_WALK_RIGHT_UP;
							else
								aniId = ID_ANI_TANUKI_JUMP_WALK_LEFT_UP;
						}
						else
						{
							if (nx >= 0)
								aniId = ID_ANI_TANUKI_JUMP_RUN_RIGHT_UP;
							else
								aniId = ID_ANI_TANUKI_JUMP_RUN_LEFT_UP;
						}
					}
				}
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_TANUKI_RUNSHELLRIGHT;
				else
					aniId = ID_ANI_TANUKI_RUNSHELLLEFT;
			}
		}
		else
		{
			if (maxPower)
			{
				if (nx >= 0)
					aniId = ID_ANI_TANUKI_JUMP_RUN_RIGHT_UP;
				else
					aniId = ID_ANI_TANUKI_JUMP_RUN_LEFT_UP;
			}
			else 
			{
				if (nx >= 0)
				{
					aniId = isFlapping ? ID_ANI_TANUKI_JUMP_WALK_RIGHT_DOWN : ID_ANI_TANUKI_JUMP_WALK_RIGHT_UP;
				}
				else
				{
					aniId = isFlapping ? ID_ANI_TANUKI_JUMP_WALK_LEFT_DOWN : ID_ANI_TANUKI_JUMP_WALK_LEFT_UP;
				}
			}
		}
	}
	else
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_TANUKI_SIT_RIGHT;
			else
				aniId = ID_ANI_TANUKI_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (!isHolding)
				{
					if (nx > 0) aniId = ID_ANI_TANUKI_IDLE_RIGHT;
					else aniId = ID_ANI_TANUKI_IDLE_LEFT;
				}
				else
				{
					if (nx > 0) aniId = ID_ANI_TANUKI_IDLESHELLRIGHT;
					else aniId = ID_ANI_TANUKI_IDLESHELLLEFT;
				}
			}
			else if (vx > 0)
			{
				if (ax < 0 && (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_RUNNING_LEFT))
				{
					if (!isHolding)	aniId = ID_ANI_TANUKI_BRACE_RIGHT;
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLLEFT;
				}

				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)
					{
						if (!maxPower) aniId = ID_ANI_TANUKI_RUNNING_RIGHT;
						else aniId = ID_ANI_TANUKI_RUNNING_RIGHT_MAXPOWER;
					}
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLRIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X || ax == -MARIO_DECEL_WALK_X)
				{
					if (!isHolding) aniId = ID_ANI_TANUKI_WALKING_RIGHT;
					else aniId = ID_ANI_TANUKI_RUNSHELLRIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && (state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_RUNNING_RIGHT))
				{
					if (!isHolding)	aniId = ID_ANI_TANUKI_BRACE_LEFT;
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLRIGHT;
				}
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)
					{
						if (!maxPower) aniId = ID_ANI_TANUKI_RUNNING_LEFT;
						else aniId = ID_ANI_TANUKI_RUNNING_LEFT_MAXPOWER;
					}
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLLEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X || ax == MARIO_DECEL_WALK_X)
				{
					if (!isHolding) aniId = ID_ANI_TANUKI_WALKING_LEFT;
					else aniId = ID_ANI_TANUKI_RUNSHELLLEFT;
				}
			}
	}
		
	if (aniId == -1)
	{
		if (nx > 0)
		{
			if (!isHolding) aniId = ID_ANI_TANUKI_IDLE_RIGHT;
			else aniId = ID_ANI_TANUKI_IDLESHELLRIGHT;
		}
		else
		{
			if (!isHolding) aniId = ID_ANI_TANUKI_IDLE_LEFT;
			else aniId = ID_ANI_TANUKI_IDLESHELLLEFT;
		}
		
	}

	if (kick_start > 0)
	{
		if (nx > 0)
			aniId = ID_ANI_TANUKI_KICK_RIGHT;
		else
			aniId = ID_ANI_TANUKI_KICK_LEFT;
	}
	if (tailattack_start > 0)
	{
		if (nx > 0)
			aniId = ID_ANI_TANUKI_TAILATTACKRIGHT;
		else
			aniId = ID_ANI_TANUKI_TAILATTACKLEFT;
	}
	if (state == MARIO_STATE_ENTER_PIPE || state == MARIO_STATE_EXIT_PIPE)
		aniId = ID_ANI_TANUKI_STAND;
	return aniId;
}

void CMario::Render()
{
	if (untouchable)
	{
		// Cứ mỗi 100ms thì chớp tắt 1 lần (render 1 lần, bỏ qua 1 lần)
		if ((GetTickCount64() / 100) % 2 == 0)
		{
			return;
		}
	}

	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	
	
		if (state == MARIO_STATE_DIE)
			aniId = ID_ANI_MARIO_DIE;
		else if (isTransforming)
		{
			// Giai đoạn biến hình: chọn anim theo transform type
			if (transform_from == MARIO_LEVEL_BIG && transform_to == MARIO_LEVEL_TANUKI)
			{
				aniId = ID_ANI_TRANSFORM_TANUKI;
			}
			else if (transform_from == MARIO_LEVEL_TANUKI && transform_to == MARIO_LEVEL_BIG)
			{
				aniId = ID_ANI_TRANSFORM_TANUKI;
			}
			else if (transform_from == MARIO_LEVEL_SMALL && transform_to == MARIO_LEVEL_BIG)
			{
				aniId = (nx > 0) ? ID_ANI_TRANSFORM_BIG_RIGHT : ID_ANI_TRANSFORM_BIG_LEFT;
			}
			else if (transform_from == MARIO_LEVEL_BIG && transform_to == MARIO_LEVEL_SMALL)
			{
				aniId = (nx > 0) ? ID_ANI_TRANSFORM_SMALL_RIGHT : ID_ANI_TRANSFORM_SMALL_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_BIG)
			aniId = GetAniIdBig();
		else if (level == MARIO_LEVEL_SMALL)
			aniId = GetAniIdSmall();
		else if (level == MARIO_LEVEL_TANUKI)
			aniId = GetAniIdTanuki();
		
	/*DebugOutTitle(L"vx: %d", aniId);*/
	animations->Get(aniId)->Render(x, y);
	
	/*RenderBoundingBox();*/
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		if (vx < 0) isTurning = true;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		if (vx > 0) isTurning = true;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		if (vx < 0) isTurning = true;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		if (vx > 0) isTurning = true;
		break;
	case MARIO_STATE_KICK_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		kick_start = GetTickCount64();  // Đánh dấu thời điểm đá
		break;
	case MARIO_STATE_KICK_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		kick_start = GetTickCount64();  // Đánh dấu thời điểm đá
		break;
	case TANUKI_STATE_TAILATTACK:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		tailattack_start = GetTickCount64(); 
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (level != MARIO_LEVEL_TANUKI)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
					vy = -MARIO_JUMP_RUN_SPEED_Y;
				else
					vy = -MARIO_JUMP_SPEED_Y;
			}
			else
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
					vy = -TANUKI_JUMP_RUN_SPEED_Y;
				else
					vy = -TANUKI_JUMP_SPEED_Y;
			}
		}
		else
		{
			if (level == MARIO_LEVEL_TANUKI)
			{
				if (maxPower)
				{
					/*vy = -TANUKI_FLAP_SPEED_Y;*/
					ay = -TANUKI_GRAVITY / 1.25f;
					if (!isFlying)
					{
						isFlying = true;
						flying_start = GetTickCount64();
					}
				}
				else
				{
					if (vy > 0)
					{
						const float SLOW_FALL_GRAVITY = MARIO_GRAVITY * 0.001f;
						const float MAX_SLOW_FALL_SPEED = 0.05f;
						const float MIN_SLOW_FALL_SPEED = 0.04f;

						if (vy > MAX_SLOW_FALL_SPEED)
							vy = MAX_SLOW_FALL_SPEED;
						if (vy < MIN_SLOW_FALL_SPEED)
							vy = MIN_SLOW_FALL_SPEED;
						ay = SLOW_FALL_GRAVITY;

						if (!isFloating)
						{
							isFloating = true;
							floating_start = GetTickCount64();
						}
					}
					
				}
				isFlapping = true;
				flap_start = GetTickCount64();
			}
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0)
		{
			if(level != MARIO_LEVEL_TANUKI)	vy += MARIO_JUMP_SPEED_Y / 2;
			else
			{
				if (!isFlying) vy += TANUKI_JUMP_RUN_SPEED_Y / 2;
				else vy += TANUKI_FLAP_SPEED_Y / 4;	
			}
		}
		if (level == MARIO_LEVEL_TANUKI)
		{
			isFloating = false;
			isFlapping = false;
			ay = TANUKI_GRAVITY; // Đặt lại trọng lực bình thường
		}
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			if (level != MARIO_LEVEL_TANUKI) y += MARIO_SIT_HEIGHT_ADJUST;
			else y += MARIO_SIT_HEIGHT_ADJUST_TANUKI;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			if (level != MARIO_LEVEL_TANUKI) y -= MARIO_SIT_HEIGHT_ADJUST;
			else y -= MARIO_SIT_HEIGHT_ADJUST_TANUKI;
		}
		break;

	case MARIO_STATE_IDLE:
		if (nx > 0) ax = -MARIO_DECEL_WALK_X;
		else ax = MARIO_DECEL_WALK_X;
		if (level == MARIO_LEVEL_TANUKI) ay = TANUKI_GRAVITY;
		else ay = MARIO_GRAVITY;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_ENTER_PIPE:
		enterPipeStart = GetTickCount64(); 
		vx = 0; 
		ax = 0;
		break;
	case MARIO_STATE_EXIT_PIPE:
		vx = 0;
		ax = 0;
		ay = 0;
		break;
	}
	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_TANUKI)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_TANUKI_BBOX_WIDTH / 2;
			top = y - MARIO_TANUKI_BBOX_HEIGHT / 2;
			right = left + MARIO_TANUKI_BBOX_WIDTH;
			bottom = top + MARIO_TANUKI_BBOX_HEIGHT;
		}
	}
	else if(level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	if (level == MARIO_LEVEL_SMALL && l == MARIO_LEVEL_BIG && !finishTransforming)
	{
		isTransforming = true;
		transform_start = GetTickCount64();
		transform_from = level;
		transform_to = l;
		return;
	}
	if (level == MARIO_LEVEL_BIG && l == MARIO_LEVEL_SMALL && !finishTransforming)
	{
		isTransforming = true;
		transform_start = GetTickCount64();
		transform_from = level;
		transform_to = l;

		StartUntouchable();

		return;
	}
	if (level == MARIO_LEVEL_BIG && l == MARIO_LEVEL_TANUKI && !finishTransforming)
	{
		isTransforming = true;
		transform_start = GetTickCount64();
		transform_from = level;
		transform_to = l;
		return;
	}
	if (level == MARIO_LEVEL_TANUKI && l == MARIO_LEVEL_BIG && !finishTransforming)
	{
		isTransforming = true;
		transform_start = GetTickCount64();
		transform_from = level;
		transform_to = l;

		StartUntouchable();

		return;
	}
	finishTransforming = false;
	int oldHeight, newHeight;

	// Lấy chiều cao cũ
	if (level == MARIO_LEVEL_SMALL)
		oldHeight = MARIO_SMALL_BBOX_HEIGHT;
	else if (level == MARIO_LEVEL_TANUKI)
		oldHeight = MARIO_TANUKI_BBOX_HEIGHT;
	else // Mặc định BIG
		oldHeight = MARIO_BIG_BBOX_HEIGHT;

	// Lấy chiều cao mới
	if (l == MARIO_LEVEL_SMALL)
		newHeight = MARIO_SMALL_BBOX_HEIGHT;
	else if (l == MARIO_LEVEL_TANUKI)
		newHeight = MARIO_TANUKI_BBOX_HEIGHT;
	else // Mặc định BIG
		newHeight = MARIO_BIG_BBOX_HEIGHT;

	if (l == MARIO_LEVEL_TANUKI)
	{
		ay = TANUKI_GRAVITY;
	}
	else
	{
		ay = MARIO_GRAVITY;
	}
		
	// Điều chỉnh y để không rơi xuyên nền
	y -= (newHeight - oldHeight) / 2.0f;

	level = l;
}



