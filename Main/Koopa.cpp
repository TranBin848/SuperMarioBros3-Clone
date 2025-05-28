#include "Koopa.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Mario.h"
#include "ItemBox.h"
#include "Brick.h"
#include "BrickBreakEffect.h"
#include "HitEnemyEffect.h"
CKoopa::CKoopa(float x, float y, int flag) :CGameObject(x, y)
{
	sensor = new CKoopaSensor(x, y);
	sensor->SetOwner(this);
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene)
	{
		scene->AddObject(sensor); // hoặc push vào vector<objects> tùy bạn tổ chức
	}
	level = flag;
	this->ax = 0;
	if (level == KOOPA_LEVEL_RED || level == KOOPA_LEVEL_GREEN)
	{
		this->ay = KOOPA_GRAVITY;
	}
	else
	{
		this->ay = PARAKOOPA_GRAVITY;
	}
	die_start = -1;
	shell_start = 0;
	return_start = 0;
	just_activated = false;
	this->renderLayer = 5;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_ACTIVATE || state == KOOPA_STATE_RETURN)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
	}
	else if (state == KOOPA_STATE_WALKING)
	{
		if (level == KOOPA_LEVEL_RED || level == KOOPA_LEVEL_GREEN)
		{
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - KOOPA_BBOX_HEIGHT / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + KOOPA_BBOX_HEIGHT;
		}
		else
		{
			left = x - KOOPA_BBOX_WIDTH / 2;
			top = y - PARAKOOPA_BBOX_HEIGHT / 2;
			right = left + KOOPA_BBOX_WIDTH;
			bottom = top + PARAKOOPA_BBOX_HEIGHT;
		}
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_ACTIVATE)
	{
		if (dynamic_cast<CGoomba*>(e->obj))
		{
			OnCollisionWithGoomba(e);
			return;
		}
		else if (dynamic_cast<CParaGoomba*>(e->obj))
		{
			OnCollisionWithParaGoomba(e);
			return;
		}
		if (dynamic_cast<CKoopa*>(e->obj))
		{
			OnCollisionWithKoopa(e);
			return;
		}
		if (dynamic_cast<CBrick*>(e->obj))
		{
			OnCollisionWithShinyBrick(e);
			return;
		}
	}
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		isOnGround = true;
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CItemBox*>(e->obj) && state == KOOPA_STATE_ACTIVATE)
		OnCollisionWithItemBox(e);
}
void CKoopa::OnCollisionWithItemBox(LPCOLLISIONEVENT e) {
	CItemBox* itb = dynamic_cast<CItemBox*>(e->obj);
	if (!itb) return;
	if (itb->GetState() == ITEMBOX_STATE_IDLE && state != KOOPA_STATE_WALKING) {
		float itbx, itby;
		itb->GetPosition(itbx, itby);
		float MarioX = CMario::GetInstance()->GetX();
		
		
		int direction = MarioX < itbx ? 1 : -1; // 1 là từ trái, -1 là từ phải
		itb->SetBounceDirection(direction); // Thêm hàm này
		itb->SetState(ITEMBOX_STATE_BOUNCING);
	}
}
void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() != GOOMBA_STATE_DIEBYSHELL)
	{
		goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
		LPGAMEOBJECT effect = nullptr;
		effect = new CHitEnemyEffect(x, y);
		if (effect)
		{
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (scene)
			{
				scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
			}
		}
	}
}
void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* kp = dynamic_cast<CKoopa*>(e->obj);
	if (kp->GetState() != KOOPA_STATE_DIEBYSHELL)
	{
		kp->SetState(KOOPA_STATE_DIEBYSHELL);
		LPGAMEOBJECT effect = nullptr;
		effect = new CHitEnemyEffect(x, y);
		if (effect)
		{
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (scene)
			{
				scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
			}
		}
	}
}
void CKoopa::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (goomba->GetState() != PARAGOOMBA_STATE_DIEBYSHELL)
	{
		goomba->SetState(PARAGOOMBA_STATE_DIEBYSHELL);
		LPGAMEOBJECT effect = nullptr;
		effect = new CHitEnemyEffect(x, y);
		if (effect)
		{
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (scene)
			{
				scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
			}
		}
	}
}
void CKoopa::OnCollisionWithShinyBrick(LPCOLLISIONEVENT e)
{
	if (this->state != KOOPA_STATE_ACTIVATE)
		return;
	// Chỉ xử lý nếu va chạm ngang (ko xử lý nếu va chạm từ trên hoặc dưới)
	if (e->ny != 0)
		return;
	vx = -vx;
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (brick == nullptr) return;

	// Tạo hiệu ứng vỡ gạch
	LPGAMEOBJECT effect = nullptr;
	effect = new CBrickBreakEffect(brick->GetX(), brick->GetY());
	if (effect)
	{
		CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (scene)
		{
			scene->InsertObjectBefore(this, effect); // hoặc push vào vector<objects> tùy bạn tổ chức
			
		}
	}

	// Đánh dấu gạch bị phá
	brick->isDeleted = true;
}
void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActivated) return;
	if (isBeingHeld)
	{
		if (state == KOOPA_STATE_SHELL)
		{
			if (GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT)
			{
				SetState(KOOPA_STATE_RETURN);
			}
		}
		else if (state == KOOPA_STATE_RETURN)
		{
			if (GetTickCount64() - return_start > KOOPA_SHELL_RECOVER)
			{
				SetState(KOOPA_STATE_WALKING);
				isBeingHeld = false;
				vy = -KOOPA_JUMP_SPEED;
				CMario::GetInstance()->TakeDmg();
				return;
			}
		}
		float mx, my, mnx, max, may, mvx, mvy;
		CMario::GetInstance()->GetPosition(mx, my);
		CMario::GetInstance()->GetDirection(mnx);
		CMario::GetInstance()->GetAcc(max, may);
		CMario::GetInstance()->GetSpeed(mvx, mvy);
		if (mvy == 0) may = 0;
		mvy += may * dt;
		mvx += max * dt;

		mx += mvx * dt;
		my += mvy * dt;

		float ox, oy;
		CMario::GetInstance()->GetShellOffset(ox, oy);

		float targetX = (mnx >= 0) ? (mx + ox) : (mx - ox);
		float targetY = my + oy;

		float dx = targetX - x;
		float dy = targetY - y;
		if (dt != 0)
			SetSpeed(dx / dt, dy / dt);
		x += vx * dt;
		y += vy * dt;
	}	
	else
	{
		if (state == KOOPA_STATE_DIEBYSHELL)
		{
			vy += ay * dt;
			if (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT)
			{
				sensor->isDeleted = true;
				isDeleted = true;
				return;
			}
		}
		if (level == KOOPA_LEVEL_RED || level == KOOPA_LEVEL_GREEN)
		{
			if (state == KOOPA_STATE_ACTIVATE)
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
			else if (state == KOOPA_STATE_SHELL)
			{
				vx = 0;
	
				if (just_activated)
				{
					just_activated = false; // Đặt lại flag sau frame đầu tiên
				}
				else
				{
					vy += ay * dt; // Chỉ cập nhật vy từ frame thứ hai trở đi
				}
				if (GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT)
				{
					SetState(KOOPA_STATE_RETURN);
				}
			}
			else if (state == KOOPA_STATE_RETURN)
			{
				vx = 0;
				vy = 0;
				if (GetTickCount64() - return_start > KOOPA_SHELL_RECOVER)
				{
					SetState(KOOPA_STATE_WALKING);
				}
			}
			if (state == KOOPA_STATE_WALKING)
			{
				vx += ax * dt;
				vy += ay * dt;
			}
		}
		else if (level == KOOPA_LEVEL_PARA)
		{
			if (state == KOOPA_STATE_WALKING)
			{
				vx += ax * dt;
				if (isOnGround)
				{
					vy = -PARAKOOPA_JUMP_SPEED; // Nhảy lên
					isOnGround = false;
				}
				vy += ay * dt;
			}
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	/*DebugOutTitle(L"STATE: %d", state);*/

}

int CKoopa::GetAniIdRed()
{
	int aniId = -1;
	aniId = ID_ANI_KOOPA_WALKING;
	if (state == KOOPA_STATE_SHELL)
	{
		aniId = ID_ANI_KOOPA_SHELL;
	}
	if (state == KOOPA_STATE_ACTIVATE) 
	{
		aniId = ID_ANI_KOOPA_ACTIVATE;
	}
	else if (state == KOOPA_STATE_RETURN)
	{
		aniId = ID_ANI_KOOPA_RETURN;
	}
	else if (state == KOOPA_STATE_DIEBYSHELL)
	{
		aniId = ID_ANI_KOOPA_DIEBYSHELL;
	}
	if (vx > 0 && state == KOOPA_STATE_WALKING)	aniId += 100;
	if(state != KOOPA_STATE_WALKING && state != KOOPA_STATE_DIEBYSHELL)
		if(isUpsideDown)
			aniId += 5;
	return aniId;
}
int CKoopa::GetAniIdGreen()
{
	int aniId = -1;
	aniId = ID_ANI_GRKOOPA_WALKING;
	if (state == KOOPA_STATE_SHELL)
	{
		aniId = ID_ANI_GRKOOPA_SHELL;
	}
	if (state == KOOPA_STATE_ACTIVATE)
	{
		aniId = ID_ANI_GRKOOPA_ACTIVATE;
	}
	else if (state == KOOPA_STATE_RETURN)
	{
		aniId = ID_ANI_GRKOOPA_RETURN;
	}
	else if (state == KOOPA_STATE_DIEBYSHELL)
	{
		aniId = ID_ANI_GRKOOPA_DIEBYSHELL;
	}
	if (vx > 0 && state == KOOPA_STATE_WALKING)	aniId += 100;
	if (state != KOOPA_STATE_WALKING && state != KOOPA_STATE_DIEBYSHELL)
		if (isUpsideDown)
			aniId += 5;
	return aniId;
}
int CKoopa::GetAniIdPara()
{
	int aniId = -1;
	aniId = ID_ANI_PARAKOOPA_WALKING;
	if (state == KOOPA_STATE_DIEBYSHELL)
	{
		aniId = ID_ANI_GRKOOPA_DIEBYSHELL;
	}
	if (vx > 0 && state == KOOPA_STATE_WALKING)	aniId += 100;
	return aniId;
}

void CKoopa::Render()
{
	int aniId = -1;
	if (level == KOOPA_LEVEL_RED)
		aniId = GetAniIdRed();
	else if (level == KOOPA_LEVEL_GREEN)
		aniId = GetAniIdGreen();
	else if (level == KOOPA_LEVEL_PARA)
		aniId = GetAniIdPara();
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CKoopa::SetState(int state)
{
	// Nếu chuyển từ dạng cao (walking) sang shell/activate thì mới chỉnh y
	if ((this->state != KOOPA_STATE_SHELL && this->state != KOOPA_STATE_ACTIVATE) &&
		(state == KOOPA_STATE_SHELL || state == KOOPA_STATE_ACTIVATE))
	{
		y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL) / 2.0f;
	}
	else if ((this->state == KOOPA_STATE_SHELL || this->state == KOOPA_STATE_ACTIVATE || this -> state == KOOPA_STATE_RETURN) &&
		state == KOOPA_STATE_WALKING)
	{
		// Ngược lại, từ shell/activate -> walking thì phải nâng Koopa lên
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL) / 2.0f;
	}
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_SHELL:
		shell_start = GetTickCount64(); // Ghi lại thời gian vào shell
		just_activated = true;
		ay = KOOPA_GRAVITY;
		y -= 2.0f;
		break;
	case KOOPA_STATE_WALKING:
		shell_start = 0; // Không cần đếm thời gian nữa
		vx = (vx >= 0) ? KOOPA_WALKING_SPEED : -KOOPA_WALKING_SPEED; // hướng chạy tiếp
		if (level == KOOPA_LEVEL_PARA) ay = PARAKOOPA_GRAVITY;
		else ay = KOOPA_GRAVITY;
		just_activated = false;
		break;
	case KOOPA_STATE_ACTIVATE:
		vx = (vx >= 0) ? KOOPA_ACTIVATE_SPEED : -KOOPA_ACTIVATE_SPEED; // hướng chạy tiếp
		y -= 2.0f;
		ay = KOOPA_GRAVITY;
		just_activated = true;
		break;
	case KOOPA_STATE_RETURN:
		vx = 0;
		return_start = GetTickCount64(); // Ghi lại thời điểm bắt đầu return
		just_activated = false;
		ay = KOOPA_GRAVITY;
		break;
	case KOOPA_STATE_DIEBYSHELL:
		isBeingHeld = false;
		die_start = GetTickCount64();
		vy = -0.3f;
		ay = KOOPA_GRAVITY;
		break;
	}
}
void CKoopa::SetLevel(int l)
{
	int oldHeight, newHeight;

	// Lấy chiều cao cũ
	if (level == KOOPA_LEVEL_RED || level == KOOPA_LEVEL_GREEN)
		oldHeight = KOOPA_BBOX_HEIGHT;
	else 
		oldHeight = PARAKOOPA_BBOX_HEIGHT;

	// Lấy chiều cao mới
	if (l == KOOPA_LEVEL_RED || l == KOOPA_LEVEL_GREEN)
		newHeight = KOOPA_BBOX_HEIGHT;
	else 
		newHeight = PARAKOOPA_BBOX_HEIGHT;

	// Điều chỉnh y để không rơi xuyên nền
	y -= (newHeight - oldHeight) / 2.0f;

	level = l;
}