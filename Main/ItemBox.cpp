#include "ItemBox.h"
#include "Mario.h"
#include "Giant.h"
#include "Leaf.h"
#include "PlayScene.h"
#include "Coin.h"
void CItemBox::Render()
{
	int aniId = ID_ANI_ITEMBOX_IDLE;
    if (flagCoin == 2) aniId = ID_ANI_ITEMBOXBRICK_IDLE;
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
                LPGAMEOBJECT newItem = nullptr;
                if (flagCoin == 0)
                {
                    for (LPGAMEOBJECT obj : *coObjects)
                    {
                        CMario* mario = dynamic_cast<CMario*>(obj);
                        if (mario)
                        {
                            int level = mario->GetLevel();
                            
                            if (level == MARIO_LEVEL_SMALL)
                            {
                                newItem = new CGiant(x, y, false); // xuất hiện phía trên
                                if (newItem->GetState() == GIANT_STATE_IDLE)
                                {
                                    newItem->SetState(GIANT_STATE_ACTIVATE);
                                }
                            }
                            else if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_TANUKI)
                            {
                                newItem = new CLeaf(x, y);
                                if (newItem->GetState() == LEAF_STATE_IDLE)
                                {
                                    newItem->SetState(LEAF_STATE_RISING);
                                }
                            }
                            break; // chỉ xử lý 1 Mario
                        }
                    }
                }
                else if(flagCoin == 1)
                {
                    newItem = new CCoin(x, y);
                    if (newItem->GetState() == COIN_STATE_IDLE)
                    {
                        newItem->SetState(COIN_STATE_BOUNCING);
                    }
                }
                else if (flagCoin == 2)
                {
                    newItem = new CGiant(x, y, true); // xuất hiện phía trên
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
            }
        }
        else {
            y -= ITEMBOX_BOUNCE_SPEED * dt; // Di chuyển lên trên
            if (y < originalY - 14.0f)
            {
                pickable = false;
                y = originalY - 14.0f;
            }
        }
        CGameObject::Update(dt, coObjects); 
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
}