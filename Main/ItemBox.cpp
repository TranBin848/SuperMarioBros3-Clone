#include "ItemBox.h"
#include "Mario.h"
#include "Giant.h"
#include "PlayScene.h"
void CItemBox::Render()
{
	int aniId = ID_ANI_ITEMBOX_IDLE;
	if (state == ITEMBOX_STATE_BOUNCING || state == ITEMBOX_STATE_ACTIVATE)
	{
		aniId = ID_ANI_ITEMBOX_BOUNCING;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CItemBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEMBOX_BBOX_WIDTH / 2;
	t = y - ITEMBOX_BBOX_HEIGHT / 2;
	r = l + ITEMBOX_BBOX_WIDTH;
	b = t + ITEMBOX_BBOX_HEIGHT;
}

void CItemBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
    /*if (dynamic_cast<CGiant*>(e->obj))
    {
        if (state != ITEMBOX_STATE_ACTIVATE) return;
        CGiant* giant = dynamic_cast<CGiant*>(e -> obj);
        if (giant)
        {
            if (giant->GetState() == GIANT_STATE_IDLE)
            {
                giant->SetState(GIANT_STATE_ACTIVATE);
            }
        }
    }*/
}

void CItemBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == ITEMBOX_STATE_BOUNCING)
    {
        if (!pickable) {
            if (y < originalY)
            {
                y += ITEMBOX_BOUNCE_SPEED * dt;
            }
            if (y > originalY) {
                y = originalY;
                SetState(ITEMBOX_STATE_ACTIVATE);
                for (LPGAMEOBJECT obj : *coObjects)
                {
                    CMario* mario = dynamic_cast<CMario*>(obj);
                    if (mario)
                    {
                        int level = mario->GetLevel();
                        LPGAMEOBJECT newItem = nullptr;

                        if (level == MARIO_LEVEL_SMALL)
                        {
                            newItem = new CGiant(x, y); // xuất hiện phía trên
                            if (newItem->GetState() == GIANT_STATE_IDLE)
                            {
                                newItem->SetState(GIANT_STATE_ACTIVATE);
                            }
                        }
                        else if (level == MARIO_LEVEL_BIG)
                        {
                            newItem = new CGiant(x, y);
                            if (newItem->GetState() == GIANT_STATE_IDLE)
                            {
                                newItem->SetState(GIANT_STATE_ACTIVATE);
                            }
                        }

                        if (newItem)
                        {
                            CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
                            if (scene)
                            {
                                scene->InsertObjectBefore(this, newItem); // hoặc push vào vector<objects> tùy bạn tổ chức
                            }
                        }
                        break; // chỉ xử lý 1 Mario
                    }
                }
            }
        }
        else {
            y -= ITEMBOX_BOUNCE_SPEED * dt; // Di chuyển lên trên
            if (y < originalY - ITEMBOX_BOUNCE_HEIGHT)
            {
                pickable = false;
                y = originalY - ITEMBOX_BOUNCE_HEIGHT;
            }
        }
    }
}