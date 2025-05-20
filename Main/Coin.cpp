#include "Coin.h"

void CCoin::Render()
{
	int aniId = ID_ANI_COIN;
	if (state == COIN_STATE_ACTIVATE || state == COIN_STATE_BOUNCING)
	{
		aniId = ID_ANI_COIN_ACTIVATE;
	}
	if (isIdleCoin) aniId = ID_ANI_COIN_IDLE;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == COIN_STATE_BOUNCING)
	{
		y -= COIN_BOUNCE_SPEED * dt; // Di chuyển lên trên
		if (y < originalY - COIN_BBOX_HEIGHT - 46.0f)
		{
			y = originalY - COIN_BBOX_HEIGHT - 46.0f;
			state = COIN_STATE_ACTIVATE;
		}
	}
	else if (state == COIN_STATE_ACTIVATE) {
		if (y < originalY)
		{
			y += COIN_GRAVITY * dt;
		}
		if (y > originalY - 26.0f) {
			y = originalY - 26.0f;
			Delete();
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}