#include "BrickBreakEffect.h"
#include "debug.h"
CBrickBreakEffect::CBrickBreakEffect(float x, float y)
{
    float vx = 50.0f;    // tốc độ ngang
    float vy = 200.0f;   // tốc độ bay lên
    float lifetime = 1.5f;

    LPANIMATION anim = CAnimations::GetInstance()->Get(ID_BRICK_PIE);
    if (!anim)
    {
        DebugOutTitle(L"[ERROR] Brick break animation not found");
        return;
    }

    // Bay lên và lệch trái/phải, sau đó rơi
    AddParticle(new CParticle(x, y, -vx, -vy, lifetime, anim)); // Trái trên
    AddParticle(new CParticle(x, y, vx, -vy, lifetime, anim));  // Phải trên
    AddParticle(new CParticle(x, y, -vx * 0.5f, -vy * 0.5f, lifetime, anim)); // Trái dưới
    AddParticle(new CParticle(x, y, vx * 0.5f, -vy * 0.5f, lifetime, anim));  // Phải dưới
}
void CBrickBreakEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = 0;
    r = 0;
    b = 0;
}