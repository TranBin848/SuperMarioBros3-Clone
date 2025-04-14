#include "VenusFire.h"
#include "Mario.h"
#include "Fire.h"
#include "PlayScene.h"
void CVenusFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - VENUSFIRE_BBOX_WIDTH / 2;
	t = y - VENUSFIRE_BBOX_HEIGHT / 2;
	r = l + VENUSFIRE_BBOX_WIDTH;
	b = t + VENUSFIRE_BBOX_HEIGHT;
}

void CVenusFire::OnNoCollision(DWORD dt)
{
	
};

void CVenusFire::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	
}

void CVenusFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	float marioX, marioY;
	CMario::GetInstance()->GetPosition(marioX, marioY);
	float dx = abs(marioX - x);

	if (dx <= 150.0f)
	{
		if (isWaiting)
		{
			if (GetTickCount64() - wait_start > VENUSFIRE_WAIT_TIME)
			{
				isWaiting = false;
				isGoingUp = !isGoingUp;
				hasShot = false; // reset trạng thái bắn
			}
			// Đang chờ, không làm gì cả
			return;
		}

		if (isGoingUp)
		{
			y -= VENUSFIRE_ARISE_SPEED * dt;
			if (y <= originalY - VENUSFIRE_BBOX_HEIGHT)
			{
				y = originalY - VENUSFIRE_BBOX_HEIGHT;

				if (!isAtTop)
				{
					isAtTop = true;
					top_start = GetTickCount64(); // bắt đầu đếm thời gian chờ
				}
				else
				{
					if (!hasShot && GetTickCount64() - top_start >= VENUSFIRE_TOP_WAIT)
					{
						// BẮN FIRE Ở ĐÂY
						state = VENUSFIRE_STATE_SHOOT;
						float fireX = x;
						float fireY = y - 5.0f;
						LPGAMEOBJECT fire = new CFire(fireX, fireY);

						float dx = marioX - x;
						float dy = marioY - y;

						// Tính góc giữa vector (dx, dy) và trục Y
						float angleRad = atan2(dx, -dy); // trục Y hướng lên nên dùng -dy
						float angleDeg = angleRad * 180.0f / 3.14159265f; // rad -> độ

						// Chuẩn hóa góc về khoảng [0, 180] nếu cần
						if (angleDeg < 0) angleDeg += 360;


						// Chọn danh sách góc theo hướng Mario
						const float* angleList = (marioX >= x) ? FIXED_ANGLES_RIGHT : FIXED_ANGLES_LEFT;

						float bestAngle = angleList[0];
						float minDiff = abs(angleDeg - angleList[0]);

						for (int i = 1; i < NUM_ANGLES; i++)
						{
							float diff = abs(angleDeg - angleList[i]);
							if (diff < minDiff)
							{
								minDiff = diff;
								bestAngle = angleList[i];
							}
						}

						// Chuyển góc thành radian để tính vector hướng
						float rad = bestAngle * 3.14159265f / 180.0f;
						float vx = sin(rad);    // X = sin(angle)
						float vy = -cos(rad);   // Y = -cos(angle), trục Y ngược hướng

						fire->SetSpeed(vx * FIRE_SPEED, vy * FIRE_SPEED);

						CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
						if (scene)
						{
							scene->AddObject(fire);
						}

						hasShot = true;
						isWaiting = true;
						wait_start = GetTickCount64();
						isAtTop = false; // reset để lần sau lên lại
					}
				}
			}
		}
		else
		{
			state = VENUSFIRE_STATE_ARISE;
			y += VENUSFIRE_ARISE_SPEED * dt;
			if (y >= originalY)
			{
				y = originalY;
				isWaiting = true;
				wait_start = GetTickCount64();
			}
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CVenusFire::Render()
{
	float marioX, marioY;
	CMario::GetInstance()->GetPosition(marioX, marioY);
	int aniId = -1;
	if (state == VENUSFIRE_STATE_ARISE)
	{
		aniId = (marioY > y) ? ID_ANI_ARISE_DOWN : ID_ANI_ARISE_UP;
		if (marioX > x) aniId += 4;
	}
	else if (state == VENUSFIRE_STATE_SHOOT)
	{
		aniId = (marioY > y) ? ID_ANI_SHOOT_DOWN : ID_ANI_SHOOT_UP;
		if (marioX > x) aniId += 4;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

//void CGoomba::SetState(int state)
//{
//	CGameObject::SetState(state);
//	switch (state)
//	{
//	case GOOMBA_STATE_DIE:
//		die_start = GetTickCount64();
//		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
//		vx = 0;
//		vy = 0;
//		ay = 0;
//		break;
//	case GOOMBA_STATE_WALKING:
//		vx = -GOOMBA_WALKING_SPEED;
//		break;
//	}
//}
