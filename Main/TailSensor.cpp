
#include "TailSensor.h"

#include "Koopa.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Platform.h"
#include "Mario.h"

void CTailSensor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
    float targetY = my + oy + 4.0f;

    float dx = targetX - x;
    float dy = targetY - y;
    if (dt != 0)
        SetSpeed(dx / dt, dy / dt);
    x += vx * dt;
    y += vy * dt;
    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (dynamic_cast<CGoomba*>(obj) || dynamic_cast<CParaGoomba*>(obj) || dynamic_cast<CKoopa*>(obj))
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
void CTailSensor::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (owner->GetLevel() == MARIO_LEVEL_TANUKI && owner -> GetState() == TANUKI_STATE_TAILATTACK)
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
void CTailSensor::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    if (goomba->GetState() != GOOMBA_STATE_DIEBYSHELL)
    {
        goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
}
void CTailSensor::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* kp = dynamic_cast<CKoopa*>(e->obj);
    if (kp->GetState() != KOOPA_STATE_DIEBYSHELL)
    {
        kp->SetState(KOOPA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);

    }
}
void CTailSensor::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
    CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);
    if (goomba->GetState() != PARAGOOMBA_STATE_DIEBYSHELL)
    {
        goomba->SetState(PARAGOOMBA_STATE_DIEBYSHELL);
        owner->SetState(KOOPA_STATE_DIEBYSHELL);
    }
}
