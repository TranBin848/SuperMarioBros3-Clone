
#include "KoopaSensor.h"

#include "Koopa.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Platform.h"
#include "Mario.h"
#include "ItemBox.h"
#include "HitEnemyEffect.h"
#include "VenusFire.h"
#include "GreenVenusFire.h"
#include "PiranhaTrap.h"
void CKoopaSensor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    bool found_ground = false;
    
    float ox, oy;
    owner->GetPosition(ox, oy);

    float sensor_offset_x = 0, sensor_offset_y = 0;

    if (!owner->GetIsBeingHeld())
    {
        float vx, vy;
        owner->GetSpeed(vx, vy);
        //4
        sensor_offset_x = (vx >= 0) ? (KOOPA_BBOX_WIDTH / 2) : (-KOOPA_BBOX_WIDTH / 2);
        sensor_offset_y = KOOPA_BBOX_HEIGHT / 2 + 3;
    }
    else
    {
        float mnx;
        CMario::GetInstance()->GetDirection(mnx);
        sensor_offset_x = (mnx >= 0) ? (KOOPA_BBOX_WIDTH / 2 + 5) : (-KOOPA_BBOX_WIDTH / 2 - 5);
        sensor_offset_y = KOOPA_BBOX_HEIGHT_SHELL / 2;
        for (LPGAMEOBJECT obj : *coObjects)
        {
            if (dynamic_cast<CGoomba*>(obj) || dynamic_cast<CParaGoomba*>(obj) || dynamic_cast<CKoopa*>(obj)
                || dynamic_cast<CVenusFire*>(obj) || dynamic_cast<CGreenVenusFire*>(obj) || dynamic_cast<CPiranhaTrap*>(obj))
            {
                float l, t, r, b;
                obj->GetBoundingBox(l, t, r, b);
                float sl, st, sr, sb;
                this->GetBoundingBox(sl, st, sr, sb);

                // Kiểm tra chồng bounding box đơn giản
                if (!(sl > r || sr < l || st > b || sb < t))
                {
                    // Ước lượng hướng va chạm đơn giản
                    float dx = obj->GetX() - this->x;
                    float dy = obj->GetY() - this->y;
                    float nx = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
                    float ny = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);

                    LPCOLLISIONEVENT fakeEvent = new CCollisionEvent(0.0f, nx, ny, 0.0f, 0.0f, obj, this);
                    OnCollisionWith(fakeEvent);
                    delete fakeEvent; 
                }
            }
                
        }
    }
    x = ox + sensor_offset_x;
    y = oy + sensor_offset_y;
    if ((owner->GetLevel() == KOOPA_LEVEL_RED && owner->GetState() == GOOMBA_STATE_WALKING) || owner->GetIsBeingHeld())
    {
        for (LPGAMEOBJECT obj : *coObjects)
        {
            if (!obj->IsBlocking()) continue;
            float l, t, r, b;
            obj->GetBoundingBox(l, t, r, b);
            if (x >= l - 2 && x <= r + 2 && y <= b + 1 && y >= t - 8)
            {
                found_ground = true;
                break;
            }
        }
        if (!found_ground)
        {
            if (!hasTurned && owner)
            {
                owner->SetVX();
                hasTurned = true;
            }
        }
        else
        {
            hasTurned = false;
        }
    }
    
    CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CKoopaSensor::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (owner -> GetIsBeingHeld() == true)
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
        }
    }
    if (dynamic_cast<CItemBox*>(e->obj))
    {
        /*DebugOutTitle(L"CHECK");*/
        OnCollisionWithItemBox(e);
        return;
    }
    else if (dynamic_cast<CVenusFire*>(e->obj) || dynamic_cast<CGreenVenusFire*>(e->obj)
        || dynamic_cast<CPiranhaTrap*>(e->obj))
    {
        OnCollisionWithDmgObject(e);
        return;
    }
    if (!e->obj->IsBlocking()) return;       
    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}
void CKoopaSensor::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    if (goomba->GetState() != GOOMBA_STATE_DIEBYSHELL && goomba ->GetState() != GOOMBA_STATE_DIE)
    {
        goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
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
void CKoopaSensor::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* kp = dynamic_cast<CKoopa*>(e->obj);
    if (kp == owner) return;
    if (kp->GetState() != KOOPA_STATE_DIEBYSHELL)
    {
        kp->SetState(KOOPA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
        
    }
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
void CKoopaSensor::OnCollisionWithDmgObject(LPCOLLISIONEVENT e)
{
    CGameObject* pl = dynamic_cast<CGameObject*>(e->obj);
    pl->isDeleted = true;
    LPGAMEOBJECT effect = nullptr;
    effect = new CHitEnemyEffect(x, y);
    owner->SetState(KOOPA_STATE_DIEBYSHELL);
    if (effect)
    {
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene)
        {
            scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
        }
    }
}
void CKoopaSensor::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
    CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);
    if (goomba->GetState() != PARAGOOMBA_STATE_DIEBYSHELL && goomba->GetState() != PARAGOOMBA_STATE_DIE)
    {
        goomba->SetState(PARAGOOMBA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
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
void CKoopaSensor::OnCollisionWithItemBox(LPCOLLISIONEVENT e) {
    CItemBox* itb = dynamic_cast<CItemBox*>(e->obj);
    if (!itb) return;
    
    if (itb->GetState() == ITEMBOX_STATE_BOUNCING) {
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
}
