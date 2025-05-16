#include "EnemyActivator.h"
#include "Mario.h"

void CEnemyActivator::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    
    float marioX, marioY;
    CMario::GetInstance()->GetPosition(marioX, marioY);
    x = marioX + 120.0f;
    y = 120.0f;
    for (auto obj : *coObjects)
    {
        if (obj == this) continue; // bỏ qua chính mình

        // Check bounding box chạm nhau
        float l1, t1, r1, b1;
        float l2, t2, r2, b2;
        this->GetBoundingBox(l1, t1, r1, b1);
        obj->GetBoundingBox(l2, t2, r2, b2);

        bool collision = !(r1 < l2 || r2 < l1 || b1 < t2 || b2 < t1);

        if (collision)
        {
            obj->Activate();
        }
    }
    DebugOutTitle(L"x: %f, y: %f", x, y);
}

void CEnemyActivator::Render()
{
    /*RenderBoundingBox();*/
}