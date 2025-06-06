
#include "TailSensor.h"

#include "Koopa.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Platform.h"
#include "Mario.h"
#include "ItemBox.h"
#include "VenusFire.h"
#include "GreenVenusFire.h"
#include "PiranhaTrap.h"
#include "HitEnemyEffect.h"
#include "Brick.h"
#include "Leaf.h"
#include "BrickBreakEffect.h"
#include "AddScoreEffect.h"

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
    CMario::GetInstance()->GetTailOffset(ox, oy, isFront);

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
        if (dynamic_cast<CGoomba*>(obj) || dynamic_cast<CParaGoomba*>(obj) || dynamic_cast<CKoopa*>(obj) 
            || dynamic_cast<CItemBox*>(obj) || dynamic_cast<CVenusFire*>(obj) || dynamic_cast<CGreenVenusFire*>(obj)
            || dynamic_cast<CPiranhaTrap*>(obj) || dynamic_cast<CBrick*>(obj) || dynamic_cast<CLeaf*>(obj))
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
    if (owner->GetLevel() == MARIO_LEVEL_TANUKI && owner -> GetState() == TANUKI_STATE_TAILATTACK && owner -> GetUntouchable() == 0)
    {
        if (dynamic_cast<CBrick*>(e->obj))
        {
            
            OnCollisionWithShinyBrick(e);
            return;
        }
        else if (dynamic_cast<CGoomba*>(e->obj))
        {
            OnCollisionWithGoomba(e);
            return;
        }
        else if (dynamic_cast<CParaGoomba*>(e->obj))
        {
            OnCollisionWithParaGoomba(e);
            return;
        }
        else if (dynamic_cast<CKoopa*>(e->obj))
        {
            OnCollisionWithKoopa(e);
            return;
        }
        else if (dynamic_cast<CItemBox*>(e->obj))
        {
            OnCollisionWithItemBox(e);
            return;
        }
        else if (dynamic_cast<CVenusFire*>(e->obj) || dynamic_cast<CGreenVenusFire*>(e->obj)
            || dynamic_cast<CPiranhaTrap*>(e->obj))
        {
            OnCollisionWithPlant(e);
            return;
        }
    }   
    if (dynamic_cast<CLeaf*>(e->obj))
        OnCollisionWithLeaf(e);
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
    if (goomba->GetState() == GOOMBA_STATE_DIE) return;
    if (goomba->GetState() != GOOMBA_STATE_DIEBYSHELL)
    {
        goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
        float ex;
        if (isFront)
            ex = x + 2;
        else
            ex = x - 2;
        LPGAMEOBJECT effect = nullptr;
        effect = new CHitEnemyEffect(ex, y);
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
void CTailSensor::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* kp = dynamic_cast<CKoopa*>(e->obj);
    kp->SetState(KOOPA_STATE_SHELL);
    float kx, ky, mx, my;
    kp->GetPosition(kx, ky);
    owner->GetPosition(mx, my);
    if (mx > kx)
    {
        kp->SetSpeed(-0.05f, -0.3f);
        kp->SetIsOnGround(false);
    }
    else
    {
        kp->SetSpeed(0.05f, -0.3f);
        kp->SetIsOnGround(false);
    }
        
    if (!kp->GetIsUpsideDown())
        kp->SetIsUpsideDown(true);
    float ex;
    if (isFront)
        ex = x + 2;
    else
        ex = x - 2;
    LPGAMEOBJECT effect = nullptr;
    effect = new CHitEnemyEffect(ex, y);
    if (effect)
    {
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene)
        {
            scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
        }
    }
}
void CTailSensor::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
    CParaGoomba* goomba = dynamic_cast<CParaGoomba*>(e->obj);
    if (goomba->GetState() == PARAGOOMBA_STATE_DIE) return;
    if (goomba->GetState() != PARAGOOMBA_STATE_DIEBYSHELL)
    {
        goomba->SetState(PARAGOOMBA_STATE_DIEBYSHELL);
        float ex;
        if (isFront)
            ex = x + 2;
        else
            ex = x - 2;
        LPGAMEOBJECT effect = nullptr;
        effect = new CHitEnemyEffect(ex, y);
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
void CTailSensor::OnCollisionWithItemBox(LPCOLLISIONEVENT e) {
    CItemBox* itb = dynamic_cast<CItemBox*>(e->obj);
    if (!itb) return;

    if (itb->GetState() == ITEMBOX_STATE_IDLE) {
        // Truyền hướng Mario chạm vào (trái/phải)
        float marioX = x;
        float itemboxX = itb->GetX();

        int direction = marioX < itemboxX ? 1 : -1; // 1 là từ trái, -1 là từ phải
        itb->SetBounceDirection(direction); // Thêm hàm này
        itb->SetState(ITEMBOX_STATE_BOUNCING);
    }
}
void CTailSensor::OnCollisionWithPlant(LPCOLLISIONEVENT e) {
    if (dynamic_cast<CVenusFire*>(e->obj))
    {
        CVenusFire* vf = dynamic_cast<CVenusFire*>(e->obj);
        if (!vf->GetIsWaiting()) vf->isDeleted = true;
    }
    else if (dynamic_cast<CGreenVenusFire*>(e->obj))
    {
        CGreenVenusFire* vf = dynamic_cast<CGreenVenusFire*>(e->obj);
        if (!vf->GetIsWaiting()) vf->isDeleted = true;
    }
    else if(dynamic_cast<CPiranhaTrap*>(e->obj))
    {
        CPiranhaTrap* vf = dynamic_cast<CPiranhaTrap*>(e->obj);
        if (!vf->GetIsWaiting()) vf->isDeleted = true;
    }
    float ex;
    if (isFront)
        ex = x + 2;
    else
        ex = x - 2;
    LPGAMEOBJECT effect = nullptr;
    effect = new CHitEnemyEffect(ex, y);
    if (effect)
    {
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene)
        {
            scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
        }
    }
}
void CTailSensor::OnCollisionWithShinyBrick(LPCOLLISIONEVENT e)
{
    // Chỉ xử lý nếu va chạm ngang (ko xử lý nếu va chạm từ trên hoặc dưới)
    /*if (e->ny != 0)
        return;*/
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
            scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
        }
    }
    // Đánh dấu gạch bị phá
    brick->isDeleted = true;
}
void CTailSensor::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
    CLeaf* l = dynamic_cast<CLeaf*>(e->obj);
    e->obj->Delete();
    if (owner->GetLevel() == MARIO_LEVEL_BIG)
        owner->SetLevel(MARIO_LEVEL_TANUKI);
    else if (owner->GetLevel() == MARIO_LEVEL_SMALL)
        owner->SetLevel(MARIO_LEVEL_BIG);
    CHUD::GetInstance()->SetScore(1000);
    LPGAMEOBJECT effect = nullptr;
    float mx, my;
    owner->GetPosition(mx, my);
    effect = new CAddScoreEffect(mx, my - 20, 1000);
    if (effect)
    {
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene)
        {
            scene->AddObject(effect); // hoặc push vào vector<objects> tùy bạn tổ chức
        }
    }
}