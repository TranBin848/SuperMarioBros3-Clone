#include "Koopa.h"
#include "ItemBox.h"
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
	else if (dynamic_cast<CItemBox*>(e->obj))
		OnCollisionWithItemBox(e);
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}
void CKoopa::OnCollisionWithItemBox(LPCOLLISIONEVENT e) {
	CItemBox* itb = dynamic_cast<CItemBox*>(e->obj);
	if (!itb) return;

	if (itb->GetState() == ITEMBOX_STATE_IDLE) {
		// Truyền hướng Mario chạm vào (trái/phải)
		float marioX = CMario::GetInstance()->GetX();
		float itemboxX = itb->GetX();

		int direction = marioX < itemboxX ? 1 : -1; // 1 là từ trái, -1 là từ phải
		itb->SetBounceDirection(direction); // Thêm hàm này
		itb->SetState(ITEMBOX_STATE_BOUNCING);
	}
}
void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (!isActivated) return;
	if (isBeingHeld)
	{
		vx = 0;
		vy = 0;
		return; // Không update vật lý khi bị cầm
	}
		
	else
	{
		if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_ACTIVATE)
		{
			vx += ax * dt;
			if (state == KOOPA_STATE_ACTIVATE && just_activated)
			{
				just_activated = false; // Đặt lại flag sau frame đầu tiên
			}
			else
			{
				vy += ay * dt; // Chỉ cập nhật vy từ frame thứ hai trở đi
			}

			// Cập nhật sensor phía trước
			if (sensor && state == KOOPA_STATE_WALKING)
			{
				float sensor_offset_x = (vx > 0 ? KOOPA_BBOX_WIDTH / 2 + 1 : -KOOPA_BBOX_WIDTH / 2 - 1);
				float sensor_x = x + sensor_offset_x;
				float sensor_y = y + KOOPA_BBOX_HEIGHT / 2 + 1;

				sensor->SetPosition(sensor_x, sensor_y);
				sensor->Update(dt, coObjects);
			}
		}

		else if (state == KOOPA_STATE_SHELL)
		{
			// Mai rùa đứng yên, không rơi
			vx = 0;
			vy = 0;
			// Kiểm tra timeout
			if (GetTickCount64() - shell_start > KOOPA_SHELL_TIMEOUT)
			{
				SetState(KOOPA_STATE_RETURN);
			}
		}
		else if (state == KOOPA_STATE_RETURN)
		{
			vx = 0;
			vy = 0;

			if (GetTickCount64() - return_start > 1000) // sau 1 giây
			{
				SetState(KOOPA_STATE_WALKING);
			}
		}
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}


void CKoopa::Render()
{
	int aniId = ID_ANI_KOOPA_WALKING;
	if (state == KOOPA_STATE_SHELL)
	{
		aniId = ID_ANI_KOOPA_SHELL;
	}
	if (state == KOOPA_STATE_ACTIVATE) {
		aniId = ID_ANI_KOOPA_ACTIVATE;
	}
	else if (state == KOOPA_STATE_RETURN)
	{
		aniId = ID_ANI_KOOPA_RETURN;
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
		vx = 0;
		shell_start = GetTickCount64(); // Ghi lại thời gian vào shell
		just_activated = false; // Thêm dòng này
		break;
	case KOOPA_STATE_WALKING:
		shell_start = 0; // Không cần đếm thời gian nữa
		vx = -KOOPA_WALKING_SPEED;
		just_activated = false; // Thêm dòng này
		break;
	case KOOPA_STATE_ACTIVATE:
		vx = (vx >= 0) ? KOOPA_ACTIVATE_SPEED : -KOOPA_ACTIVATE_SPEED; // hướng chạy tiếp
		just_activated = true; // Thêm dòng này
		break;
	case KOOPA_STATE_RETURN:
		vx = 0;
		return_start = GetTickCount64(); // Ghi lại thời điểm bắt đầu return
		just_activated = false; // Thêm dòng này
		break;

	}
}
