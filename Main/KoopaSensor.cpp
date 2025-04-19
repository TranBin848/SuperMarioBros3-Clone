#include "KoopaSensor.h"
#include "Koopa.h"
#include "Platform.h"
void CKoopaSensor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    bool found_ground = false;

    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (!obj->IsBlocking()) continue;
        float l, t, r, b;
        obj->GetBoundingBox(l, t, r, b);
        if (x >= l - 2 && x <= r + 2 && y <= b + 1 && y >= t - 8) // bạn có thể tinh chỉnh khoảng cách này
        {
            found_ground = true;
            break;
        }
    }

    if (!found_ground)
    {
        if (!hasTurned && owner)
        {
            owner->SetVX();       // đảo hướng
            hasTurned = true;     // ✅ nhớ đã quay đầu
        }
    }
    else
    {
        hasTurned = false; // ✅ reset khi quay lại mặt đất
    }
}