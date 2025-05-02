#include "ItemBox.h"
#include "ParaKoopa.h"
CParaKoopa::CParaKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = PARAKOOPA_GRAVITY;
	die_start = -1;
	shell_start = 0;
	return_start = 0;
	just_activated = false;
	this->renderLayer = 5;
	SetState(PARAKOOPA_STATE_WALKING);
}

void CParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARAKOOPA_STATE_SHELL || state == PARAKOOPA_STATE_ACTIVATE || state == PARAKOOPA_STATE_RETURN)
	{
		left = x - PARAKOOPA_BBOX_WIDTH / 2;
		top = y - PARAKOOPA_BBOX_HEIGHT_SHELL / 2;
		right = left + PARAKOOPA_BBOX_WIDTH;
		bottom = top + PARAKOOPA_BBOX_HEIGHT_SHELL;
	}
	else if (state == PARAKOOPA_STATE_WALKING)
	{
		left = x - PARAKOOPA_BBOX_WIDTH / 2;
		top = y - PARAKOOPA_BBOX_HEIGHT / 2;
		right = left + PARAKOOPA_BBOX_WIDTH;
		bottom = top + PARAKOOPA_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CParaKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CParaKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CParaKoopa*>(e->obj)) return;
	else if (dynamic_cast<CItemBox*>(e->obj) && state == PARAKOOPA_STATE_ACTIVATE)
		OnCollisionWithItemBox(e);
	if (e->ny != 0)
	{
		isOnGround = true;
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}
void CParaKoopa::OnCollisionWithItemBox(LPCOLLISIONEVENT e) {
	CItemBox* itb = dynamic_cast<CItemBox*>(e->obj);
	if (!itb) return;

	if (itb->GetState() == ITEMBOX_STATE_IDLE && state != PARAKOOPA_STATE_WALKING) {
		// Truyền hướng Mario chạm vào (trái/phải)
		float marioX = CMario::GetInstance()->GetX();
		float itemboxX = itb->GetX();

		int direction = marioX < itemboxX ? 1 : -1; // 1 là từ trái, -1 là từ phải
		itb->SetBounceDirection(direction); // Thêm hàm này
		itb->SetState(ITEMBOX_STATE_BOUNCING);
	}
}
void CParaKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (!isActivated) return;*/
	if (isBeingHeld)
	{
		vx = 0;
		vy = 0;
		return; // Không update vật lý khi bị cầm
	}
	else
	{
		if (state == PARAKOOPA_STATE_ACTIVATE)
		{
			vx += ax * dt;
			if (just_activated)
			{
				just_activated = false; // Đặt lại flag sau frame đầu tiên
			}
			else
			{
				vy += ay * dt; // Chỉ cập nhật vy từ frame thứ hai trở đi
			}
		}
		else if (state == PARAKOOPA_STATE_WALKING)
		{
			vx += ax * dt;

			if (isOnGround)
			{
				vy = -PARAKOOPA_JUMP_SPEED; // Nhảy lên
				isOnGround = false;
			}

			vy += ay * dt;
		}
		else if (state == PARAKOOPA_STATE_NORMAL_WALKING)
		{
			vx += ax * dt;
			vy += ay * dt;
		}
		else if (state == PARAKOOPA_STATE_SHELL)
		{
			vx = 0;
			vy = 0;
			// Kiểm tra timeout
			if (GetTickCount64() - shell_start > PARAKOOPA_SHELL_TIMEOUT)
			{
				SetState(PARAKOOPA_STATE_RETURN);
			}
		}
		else if (state == PARAKOOPA_STATE_RETURN)
		{
			vx = 0;
			vy = 0;

			if (GetTickCount64() - return_start > 1000) // sau 1 giây
			{
				SetState(PARAKOOPA_STATE_NORMAL_WALKING);
			}
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CParaKoopa::Render()
{
	int aniId = -1;

	aniId = ID_ANI_PARAKOOPA_WALKING;
	if (state == PARAKOOPA_STATE_NORMAL_WALKING)
	{
		aniId = ID_ANI_GRKOOPA_WALKING;
	}
	if (state == KOOPA_STATE_SHELL)
	{
		aniId = ID_ANI_GRKOOPA_SHELL;
	}
	if (state == KOOPA_STATE_ACTIVATE) {
		aniId = ID_ANI_GRKOOPA_ACTIVATE;
	}
	else if (state == KOOPA_STATE_RETURN)
	{
		aniId = ID_ANI_GRKOOPA_RETURN;
	}
	if (vx > 0)
	{
		if (state == PARAKOOPA_STATE_WALKING) aniId += 100;
		if (state == PARAKOOPA_STATE_NORMAL_WALKING) aniId += 100;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CParaKoopa::SetState(int state)
{
	// Nếu chuyển từ dạng cao (walking) sang shell/activate thì mới chỉnh y
	if ((this->state != PARAKOOPA_STATE_SHELL && this->state != PARAKOOPA_STATE_ACTIVATE) &&
		(state == PARAKOOPA_STATE_SHELL || state == PARAKOOPA_STATE_ACTIVATE))
	{
		y += (KOOPA_BBOX_HEIGHT - PARAKOOPA_BBOX_HEIGHT_SHELL) / 2.0f;
	}
	else if ((this->state == PARAKOOPA_STATE_SHELL || this->state == PARAKOOPA_STATE_ACTIVATE || this->state == PARAKOOPA_STATE_RETURN) &&
		state == PARAKOOPA_STATE_NORMAL_WALKING)
	{
		// Ngược lại, từ shell/activate -> walking thì phải nâng PARAKOOPA lên
		y -= (KOOPA_BBOX_HEIGHT - PARAKOOPA_BBOX_HEIGHT_SHELL) / 2.0f;
	}
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAKOOPA_STATE_SHELL:
		vx = 0;
		shell_start = GetTickCount64(); // Ghi lại thời gian vào shell
		just_activated = false;  
		break;
	case PARAKOOPA_STATE_NORMAL_WALKING:
		shell_start = 0; 
		vx = (vx >= 0) ? PARAKOOPA_WALKING_SPEED : -PARAKOOPA_WALKING_SPEED; // hướng chạy tiếp
		just_activated = false; 
		break;
	case PARAKOOPA_STATE_ACTIVATE:
		vx = PARAKOOPA_ACTIVATE_SPEED;	
		y -= 1.0f;
		just_activated = true; 
		break;
	case PARAKOOPA_STATE_RETURN:
		vx = 0;
		return_start = GetTickCount64(); // Ghi lại thời điểm bắt đầu return
		just_activated = false;  
		break;
	case PARAKOOPA_STATE_WALKING:
		shell_start = 0; 
		vx = -PARAKOOPA_WALKING_SPEED;
		just_activated = false;
		break;
	}
}