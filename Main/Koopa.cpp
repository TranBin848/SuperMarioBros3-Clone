#include "Koopa.h"
CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	sensor = new CKoopaSensor(x, y);
	sensor->SetOwner(this);
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene)
	{
		scene->AddObject(sensor); // hoặc push vào vector<objects> tùy bạn tổ chức
	}
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_ACTIVATE)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_ACTIVATE)
	{
		vx += ax * dt;
		vy += ay * dt;

		// Cập nhật sensor phía trước
		if (sensor)
		{
			float sensor_offset_x = (vx > 0 ? KOOPA_BBOX_WIDTH / 2 + 1 : -KOOPA_BBOX_WIDTH / 2 - 1);
			float sensor_x = x + sensor_offset_x;
			float sensor_y = y + KOOPA_BBOX_HEIGHT / 2 + 1;

			sensor->SetPosition(sensor_x, sensor_y);
			if(state == KOOPA_STATE_WALKING) sensor->Update(dt, coObjects);
		}
	}
	
	else if (state == KOOPA_STATE_SHELL)
	{
		// Mai rùa đứng yên, không rơi
		vx = 0;
		vy = 0;
	}
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}


void CKoopa::Render()
{
	int aniId = ID_ANI_KOOPA_WALKING;
	if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_ACTIVATE)
	{
		aniId = ID_ANI_KOOPA_SHELL;
	}
	if (vx > 0 && state == KOOPA_STATE_WALKING)	aniId += 100;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	// Nếu chuyển từ dạng cao (walking) sang shell/activate thì mới chỉnh y
	if ((this->state != KOOPA_STATE_SHELL && this->state != KOOPA_STATE_ACTIVATE) &&
		(state == KOOPA_STATE_SHELL || state == KOOPA_STATE_ACTIVATE))
	{
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL) / 2.0f;
	}
	else if ((this->state == KOOPA_STATE_SHELL || this->state == KOOPA_STATE_ACTIVATE) &&
		state == KOOPA_STATE_WALKING)
	{
		// Ngược lại, từ shell/activate -> walking thì phải nâng Koopa lên
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL) / 2.0f;
	}
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_SHELL:
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPA_STATE_WALKING:
		vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_ACTIVATE:
		vx = (vx >= 0) ? KOOPA_ACTIVATE_SPEED : -KOOPA_ACTIVATE_SPEED; // hướng chạy tiếp
		ay = KOOPA_GRAVITY;
		break;
	}
}
