#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"
#include "VenusFire.h"
#include "Fire.h"

#include "Coin.h"
#include "Portal.h"
#include "ItemBox.h"
#include "Giant.h"
#include "Leaf.h"
#include "Collision.h"

CMario* CMario::__instance = nullptr;
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isTransforming)
	{
		if (GetTickCount64() - transform_start >= 1000)
		{
			isTransforming = false;
			finishTransforming = true;
			SetLevel(transform_to);
		}
		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	

	if (isHolding)
	{
		LPGAME game = CGame::GetInstance();
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
			else
			{
				float offsetX = (nx > 0) ? 14.0f : -14.0f;
				float offsetY = 0.0f; // Có thể điều chỉnh để Koopa nằm thấp hơn tay một chút

				heldKoopa->SetPosition(x + offsetX, y + offsetY);
				heldKoopa->SetSpeed(0, 0);
			}
		}
		if (heldPrKoopa != nullptr)
		{
			if (!game->IsKeyDown(DIK_A))
			{
				// Thả Koopa ra
				isHolding = false;
				heldPrKoopa->SetIsBeingHeld(false);

				// Koopa chuyển sang trạng thái ACTIVE
				heldPrKoopa->SetState(PARAKOOPA_STATE_ACTIVATE);

				// Đá theo hướng Mario đang nhìn
				if (nx > 0)
				{
					SetState(MARIO_STATE_KICK_RIGHT);
					heldPrKoopa->SetSpeed(PARAKOOPA_ACTIVATE_SPEED, 0); // Koopa bay phải
				}
				else
				{
					SetState(MARIO_STATE_KICK_LEFT);
					heldPrKoopa->SetSpeed(-PARAKOOPA_ACTIVATE_SPEED, 0); // Koopa bay trái
				}
				heldPrKoopa = nullptr;
			}
			else
			{
				float offsetX = (nx > 0) ? 14.0f : -14.0f;
				float offsetY = 0.0f; // Có thể điều chỉnh để Koopa nằm thấp hơn tay một chút

				heldPrKoopa->SetPosition(x + offsetX, y + offsetY);
				heldPrKoopa->SetSpeed(0, 0);
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

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CParaGoomba*>(e->obj))
		OnCollisionWithParaGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CItemBox*>(e->obj))
		OnCollisionWithItemBox(e);
	else if (dynamic_cast<CGiant*>(e->obj))
		OnCollisonWithGiant(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CVenusFire*>(e->obj) || dynamic_cast<CFire*>(e->obj))
		OnCollisionWithDmgObject(e);
	else if (dynamic_cast<CParaKoopa*>(e->obj))
		OnCollisionWithParaKoopa(e);
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (e->ny < 0)
	{
		int koopaState = koopa->GetState();

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

		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else if (e->nx != 0) // Va chạm từ bên trái/phải
	{
		if (koopa->GetState() == KOOPA_STATE_SHELL)
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

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
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

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() == PARAGOOMBA_STATE_DIE) return;
		if (goomba->GetState() == PARAGOOMBA_STATE_NORMAL_WALKING)
		{
			goomba->SetState(PARAGOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else
		{
			goomba->SetState(PARAGOOMBA_STATE_NORMAL_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		
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
void CMario::OnCollisionWithParaKoopa(LPCOLLISIONEVENT e)
{
	CParaKoopa* koopa = dynamic_cast<CParaKoopa*>(e->obj);
	if (e->ny < 0)
	{
		int koopaState = koopa->GetState();

		switch (koopaState)
		{
		case PARAKOOPA_STATE_WALKING:
			koopa->SetState(PARAKOOPA_STATE_NORMAL_WALKING);
			break;
		case PARAKOOPA_STATE_NORMAL_WALKING:
			koopa->SetState(PARAKOOPA_STATE_SHELL);
			break;
		case KOOPA_STATE_ACTIVATE:
			koopa->SetState(KOOPA_STATE_SHELL);
			break;
		case KOOPA_STATE_SHELL:
			koopa->SetState(KOOPA_STATE_ACTIVATE);
			break;
		}

		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else if (e->nx != 0) // Va chạm từ bên trái/phải
	{
		if (koopa->GetState() == KOOPA_STATE_SHELL)
		{
			if (!isHolding && (this->state == MARIO_STATE_RUNNING_LEFT || this->state == MARIO_STATE_RUNNING_RIGHT))
			{
				// Mario cầm Koopa
				heldPrKoopa = koopa;
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
	coin++;
}

void CMario::OnCollisonWithGiant(LPCOLLISIONEVENT e)
{
	CGiant* g = dynamic_cast<CGiant*>(e->obj);
	if (g->GetState() == GIANT_STATE_ACTIVATE)
	{
		SetLevel(MARIO_LEVEL_BIG);
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* l = dynamic_cast<CLeaf*>(e->obj);
	if (l->GetState() == LEAF_STATE_FALLING || l -> GetState() == LEAF_STATE_RISING)
	{
		e->obj->Delete();
		SetLevel(MARIO_LEVEL_TANUKI);
	}
}

void CMario::OnCollisionWithDmgObject(LPCOLLISIONEVENT e) {
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

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
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
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

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
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
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
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
				{
					if (!isHolding)	aniId = ID_ANI_MARIO_BRACE_RIGHT;
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLLEFT;
				}
					
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)	aniId = ID_ANI_MARIO_RUNNING_RIGHT;
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLRIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
				{
					if (!isHolding)	aniId = ID_ANI_MARIO_BRACE_LEFT;
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLRIGHT;
				}
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (!isHolding) aniId = ID_ANI_MARIO_RUNNING_LEFT;
					else if (isHolding) aniId = ID_ANI_MARIO_RUNSHELLLEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	if (kick_start > 0)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_KICK_RIGHT;
		else
			aniId = ID_ANI_MARIO_KICK_LEFT;
	}

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
				if (nx >= 0)
					aniId = ID_ANI_TANUKI_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_TANUKI_JUMP_RUN_LEFT;
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
			if (nx >= 0)
				aniId = ID_ANI_TANUKI_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_TANUKI_JUMP_WALK_LEFT;
		}
	}
	else
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
				if (nx > 0) aniId = ID_ANI_TANUKI_IDLE_RIGHT;
				else aniId = ID_ANI_TANUKI_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
				{
					if (!isHolding)	aniId = ID_ANI_TANUKI_BRACE_RIGHT;
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLLEFT;
				}

				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (!isHolding)	aniId = ID_ANI_TANUKI_RUNNING_RIGHT;
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLRIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_TANUKI_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
				{
					if (!isHolding)	aniId = ID_ANI_TANUKI_BRACE_LEFT;
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLRIGHT;
				}
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (!isHolding) aniId = ID_ANI_TANUKI_RUNNING_LEFT;
					else if (isHolding) aniId = ID_ANI_TANUKI_RUNSHELLLEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_TANUKI_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_TANUKI_IDLE_RIGHT;

	if (kick_start > 0)
	{
		if (nx > 0)
			aniId = ID_ANI_TANUKI_KICK_RIGHT;
		else
			aniId = ID_ANI_TANUKI_KICK_LEFT;
	}

	return aniId;
}

void CMario::Render()
{
	if (untouchable)
	{
		// Cứ mỗi 100ms thì chớp tắt 1 lần (render 1 lần, bỏ qua 1 lần)
		if ((GetTickCount64() / 100) % 2 == 0)
		{
			// Không render gì cả
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
	
	animations->Get(aniId)->Render(x, y);
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
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
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

	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
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

	// Điều chỉnh y để không rơi xuyên nền
	y -= (newHeight - oldHeight) / 2.0f;

	level = l;
}

