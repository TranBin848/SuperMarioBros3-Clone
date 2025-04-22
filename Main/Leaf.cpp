#include "Leaf.h"
#include "Goomba.h"
#include "ItemBox.h"
void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = (walkingDirection > 0) ? ID_ANI_LEAF_RIGHT : ID_ANI_LEAF_LEFT;
	if (state == LEAF_STATE_IDLE)
		return; // Không render khi chưa kích hoạt
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}
void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_RISING:
		vy = -LEAF_RISE_SPEED; // Bay lên
		vx = 0;
		break;
	case LEAF_STATE_FALLING:
		startY = y; // Lưu vị trí y khi bắt đầu rơi
		sway_start = GetTickCount64();
		break;
	case LEAF_STATE_GROUNDED:
		vx = 0;
		vy = 0;
		break;
	case LEAF_STATE_IDLE:
		vx = 0;
		vy = 0;
		x = originalX;
		y = originalY;
		break;
	}
}
void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};
void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (e->ny < 0) // Lá chạm sàn từ trên xuống
	{
		vy = 0;
		vx = 0; // Dừng di chuyển ngang
	}
	else if (e->nx != 0) // Va chạm ngang (tường)
	{
		vx = -vx;
		walkingDirection = (vx > 0) ? 1 : -1;
	}
}
void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == LEAF_STATE_IDLE)
		return; // Không update khi chưa kích hoạt

	if (state == LEAF_STATE_RISING)
	{
		vy = -LEAF_RISE_SPEED; // Bay lên
		if (originalY - y >= LEAF_MAX_HEIGHT) // Đạt độ cao tối đa
		{
			SetState(LEAF_STATE_FALLING);
			sway_start = GetTickCount64(); // Bắt đầu dao động ngang
		}
	}
	else if (state == LEAF_STATE_FALLING)
	{
		vy += LEAF_GRAVITY * dt;

		if (vy > 0.05f) vy = 0.05f; // Giới hạn tốc độ rơi

		ULONGLONG now = GetTickCount64();
		float sway_phase = (float)(now - sway_start) / LEAF_SWAY_PERIOD;
		float amplitude = 32.0f;
		float sin_value = sin(2 * 3.14159f * sway_phase);

		x = originalX + amplitude * sin_value;
		walkingDirection = (sin_value > 0) ? 1 : -1;
	}
	else if (state == LEAF_STATE_GROUNDED)
	{
		vx = 0;
		vy = 0;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}