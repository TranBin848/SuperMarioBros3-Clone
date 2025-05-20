#include "SwitchBlock.h"
#include "Brick.h"
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
    if (state == SWITCHBLOCK_STATE_IDLE)
    {
        l = x - SWITCHBLOCK_BBOX_WIDTH / 2;
        t = y - SWITCHBLOCK_BBOX_HEIGHT / 2;
        r = l + SWITCHBLOCK_BBOX_WIDTH;
        b = t + SWITCHBLOCK_BBOX_HEIGHT;
    }
    else
    {
        l = x - SWITCHBLOCK_BBOX_WIDTH / 2;
        t = y - SWITCHBLOCK_BBOX_HEIGHT_ACTIVATED / 2;
        r = l + SWITCHBLOCK_BBOX_WIDTH;
        b = t + SWITCHBLOCK_BBOX_HEIGHT_ACTIVATED;
    }
}

void CSwitchBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
}

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!finishRemoveBrick && state == SWITCHBLOCK_STATE_ACTIVATED)
    {
        for (LPGAMEOBJECT obj : *coObjects)
        {
            if (dynamic_cast<CBrick*>(obj))
            {
                CBrick* br = dynamic_cast<CBrick*>(obj);
                if (br->GetCanRemove() == true)
                    br->isDeleted = true;
            }
        }
        finishRemoveBrick = true;
    }
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);   
}

void CSwitchBlock::SetState(int state) {
    currState = this->state;
    if (currState == SWITCHBLOCK_STATE_IDLE && state == SWITCHBLOCK_STATE_ACTIVATED)
    {
        float diffy = SWITCHBLOCK_BBOX_HEIGHT - SWITCHBLOCK_BBOX_HEIGHT_ACTIVATED - 2;
        y += diffy;
    }
    this->state = state; 
}