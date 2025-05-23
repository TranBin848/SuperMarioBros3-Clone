﻿#include "Coin.h"
#include "HUD.h"
#include "AddScoreEffect.h"
#include "PlayScene.h"
void CCoin::Render()
{
	int aniId = ID_ANI_COIN;
	if (state == COIN_STATE_ACTIVATE || state == COIN_STATE_BOUNCING)
	{
		aniId = ID_ANI_COIN_ACTIVATE;
	}
	if (isIdleCoin) aniId = ID_ANI_COIN_IDLE;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	/*RenderBoundingBox();*/
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
			CHUD::GetInstance()->SetScore(100);
			LPGAMEOBJECT effect = nullptr;
			effect = new CAddScoreEffect(x, y, 100);
			if (effect)
			{
				CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (scene)
				{
					scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
				}
			}
			Delete();
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}