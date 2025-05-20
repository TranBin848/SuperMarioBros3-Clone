#include "SwitchBlock.h"
#include "Mario.h"
#include "PlayScene.h"
void CSwitchBlock::Render()
{
    int aniId = ID_ANI_SWITCHBLOCK;

    if (state == SWITCHBLOCK_STATE_ACTIVATED)
    {
        aniId = ID_ANI_SWITCHBLOCK_ACTIVATED;
    }
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CSwitchBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - SWITCHBLOCK_BBOX_WIDTH / 2;
    t = y - SWITCHBLOCK_BBOX_HEIGHT / 2;
    r = l + SWITCHBLOCK_BBOX_WIDTH;
    b = t + SWITCHBLOCK_BBOX_HEIGHT;
}

void CSwitchBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
}

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);   
}