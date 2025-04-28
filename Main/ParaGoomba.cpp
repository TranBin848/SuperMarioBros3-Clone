#include "ParaGoomba.h"

CParaGoomba::CParaGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	vx = GOOMBA_WALKING_SPEED; // Mặc định đi sang trái
	nextStateTime = GetTickCount64(); // Thời gian để chuyển trạng thái tiếp theo
	lastJumpTime = GetTickCount64();
	SetState(GOOMBA_STATE_WALKING_NOWING);
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else if (state == GOOMBA_STATE_WALKING_FLAPWING || state == GOOMBA_STATE_FLY)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_WING + GOOMBA_BBOX_HEIGHT_NOWING / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_WING;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_NOWING / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_NOWING;
	}
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CParaGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (!isActivated) return;*/
	CGameObject::Update(dt, coObjects);
	ULONGLONG now = GetTickCount64();
	if (now - nextStateTime >= stateDelay)
	{
		switch (state)
		{
		case GOOMBA_STATE_WALKING_FLAPWING:
			SetState(GOOMBA_STATE_FLY);
			break;

		case GOOMBA_STATE_FLY:
			SetState(GOOMBA_STATE_WALKING_NOWING);
			break;

		case GOOMBA_STATE_WALKING_NOWING:
			SetState(GOOMBA_STATE_WALKING_FLAPWING);
			break;
		}
	}
	if (state == GOOMBA_STATE_WALKING_FLAPWING && vy == 0)
	{
		if (now - lastJumpTime > 150)
		{
			vy = -0.1f;
			lastJumpTime = now;
		}
	}
	// Update vận tốc
	vy += ay * dt;
	vx += ax * dt;

	// Xử lý chết
	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CParaGoomba::Render()
{
	int aniId = ID_ANI_PARAGOOMBA_WALKING;

	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_PARAGOOMBA_DIE;
	}
	else if (state == GOOMBA_STATE_WALKING_NOWING)
	{
		aniId = ID_ANI_PARAGOOMBA_WALKING;
	}
	else if (state == GOOMBA_STATE_WALKING_FLAPWING)
	{
		if (vx > 0)
			aniId = ID_ANI_GOOMBA_WALK_FLAPWING_RIGHT;
		else
			aniId = ID_ANI_GOOMBA_WALK_FLAPWING_LEFT;
	}
	else if (state == GOOMBA_STATE_FLY)
	{
		if (vx > 0)
			aniId = ID_ANI_GOOMBA_FLYRIGHT;
		else
			aniId = ID_ANI_GOOMBA_FLYLEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;
		break;

	case GOOMBA_STATE_WALKING_NOWING:
		vx = -GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		stateDelay = 600; 
		nextStateTime = GetTickCount64();
		break;

	case GOOMBA_STATE_WALKING_FLAPWING:
		vx = -GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		stateDelay = 900;  
		nextStateTime = GetTickCount64();
		lastJumpTime = GetTickCount64();
		break;

	case GOOMBA_STATE_FLY:
		vy = -0.35f; // Bay vút lên
		ay = GOOMBA_GRAVITY;
		stateDelay = 700; 
		nextStateTime = GetTickCount64();
		break;
	}
	
}
