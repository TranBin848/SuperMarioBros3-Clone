
#include "ItemBoxSensor.h"
#include "Koopa.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Platform.h"
#include "Mario.h"
#include "Giant.h"
#include "ItemBox.h"
void CItemBoxSensor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (dynamic_cast<CGoomba*>(obj) || dynamic_cast<CParaGoomba*>(obj) 
            || dynamic_cast<CKoopa*>(obj) || dynamic_cast<CGiant*>(obj))
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
    CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CItemBoxSensor::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (owner -> GetState() == ITEMBOX_STATE_BOUNCING)
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
        else if (dynamic_cast<CGiant*>(e->obj))
        {
            OnCollisionWithGiant(e);
        }
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
void CItemBoxSensor::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    if (goomba->GetState() != GOOMBA_STATE_DIEBYSHELL)
    {
        goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
}
void CItemBoxSensor::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* kp = dynamic_cast<CKoopa*>(e->obj);
    if (kp->GetState() != KOOPA_STATE_DIEBYSHELL)
    {
        kp->SetState(KOOPA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);

    }
}
void CItemBoxSensor::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
    CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);
    if (goomba->GetState() != PARAGOOMBA_STATE_DIEBYSHELL)
    {
        goomba->SetState(PARAGOOMBA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
}
void CItemBoxSensor::OnCollisionWithGiant(LPCOLLISIONEVENT e)
{
    CGiant* g = dynamic_cast<CGiant*>(e->obj);
    /*g->SetHitByITB();*/
    float gx, gy;
    g->GetSpeed(gx, gy);
    g->SetSpeed(gx, -0.2f);
}
