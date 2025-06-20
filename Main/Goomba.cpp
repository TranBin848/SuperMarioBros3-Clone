﻿#include "Goomba.h"
#include "Wall.h"
#include "debug.h"
CGoomba::CGoomba(float x, float y):CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};


void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 
	if (dynamic_cast<CWall*>(e->obj)) return;
	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//DebugOutTitle(L"x: %f", x);
	if (x < 20.0f) ay = 0;
	if (x < -10.0f) SetState(GOOMBA_STATE_DIE);
	if (x >= 230.0f && x <= 245.0f)
	{
		if (!isActivated) isActivated = true;
	}
	if (!isActivated) return;
	CGameObject::Update(dt, coObjects);

	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIEBYSHELL) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	else if (state == GOOMBA_STATE_DIEBYSHELL)
	{
		aniId = ID_ANI_GOOMBA_DIEBYSHELL;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	/*RenderBoundingBox();*/
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_DIEBYSHELL:
			die_start = GetTickCount64();
			vx = 0;
			vy = -0.5f; 
			ay = GOOMBA_GRAVITY; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
	}
}
int CGoomba::IsCollidable()
{
	if (x < 20.0f) return 0;
	return ((state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIEBYSHELL));
}
