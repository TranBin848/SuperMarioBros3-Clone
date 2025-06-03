#include "PiranhaTrap.h"
#include "Mario.h"
#include "Fire.h"
#include "PlayScene.h"
void CPiranhaTrap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - VENUSFIRE_BBOX_WIDTH / 2;
	t = y - VENUSFIRE_BBOX_HEIGHT / 2 + 5.0f;
	r = l + VENUSFIRE_BBOX_WIDTH;
	b = t + VENUSFIRE_BBOX_HEIGHT;
}

void CPiranhaTrap::OnNoCollision(DWORD dt)
{

};

void CPiranhaTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

}

void CPiranhaTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	float marioX, marioY;
	marioX = CMario::GetInstance()->GetX();
	marioY = CMario::GetInstance()->GetY();
	float dx = abs(marioX - x);
	bool inRange = (dx <= 230.0f);
	
	if (inRange)
	{
		if (isWaiting)
		{
			if (GetTickCount64() - wait_start > VENUSFIRE_WAIT_TIME)
			{
				isWaiting = false;
				isGoingUp = !isGoingUp;
			}
			// Đang chờ, không làm gì cả
			return;
		}
		else {
			if (dx < 24.0f)
			{
				if (y == originalY) return;
			}
		}
		if (isGoingUp)
		{
			y -= VENUSFIRE_ARISE_SPEED * dt;
			if (y <= originalY - VENUSFIRE_HEIGHT)
			{
				y = originalY - VENUSFIRE_HEIGHT;

				if (!isAtTop)
				{
					isAtTop = true;
					top_start = GetTickCount64(); // bắt đầu đếm thời gian chờ
				}
				else
				{
					if (GetTickCount64() - top_start >= VENUSFIRE_TOP_WAIT)
					{
						isWaiting = true;
						wait_start = GetTickCount64();
						isAtTop = false;
					}
				}
			}
		}
		else
		{
			y += VENUSFIRE_ARISE_SPEED * dt;
			if (y >= originalY)
			{
				y = originalY;
				isWaiting = true;
				wait_start = GetTickCount64();
			}
		}
	}
	else
	{
		if (dx > 230.0f)
		{
			y += VENUSFIRE_ARISE_SPEED * dt;
			if (y >= originalY)
			{
				isGoingUp = false;
				y = originalY;
				isWaiting = true;
				wait_start = GetTickCount64();
			}
			isWaiting = true;
			wait_start = GetTickCount64();
			isAtTop = false; // reset để lần sau lên lại
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPiranhaTrap::Render()
{
	float marioX, marioY;
	CMario::GetInstance()->GetPosition(marioX, marioY);
	int aniId = ID_ANI_PIRANHA;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	/*RenderBoundingBox();*/
}

