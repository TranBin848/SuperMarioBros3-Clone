#include "Giant.h"
#include "Goomba.h"
#include "ItemBox.h"
void CGiant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == GIANT_STATE_ACTIVATE)
		animations->Get(ID_ANI_GIANT)->Render(x, y);
}

void CGiant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GIANT_BBOX_WIDTH / 2;
	t = y - GIANT_BBOX_HEIGHT / 2;
	r = l + GIANT_BBOX_WIDTH;
	b = t + GIANT_BBOX_HEIGHT;
}
void CGiant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GIANT_STATE_IDLE:
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GIANT_STATE_ACTIVATE:
		isActivating = true;
		break;
	}
}
void CGiant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};
void CGiant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CItemBox*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}
void CGiant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isActivating)
	{
		y -= GIANT_BOUNCE_SPEED * dt;
		if (round(y) == (originalY - GIANT_BBOX_HEIGHT)) {
			isActivating = false;
			vx = -GIANT_WALKING_SPEED;
		}
	}
	if (round(x) == originalX - GIANT_BBOX_HEIGHT)
	{
		ay = GIANT_GRAVITY;
	}
	vy += ay * dt;
	vx += ax * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}