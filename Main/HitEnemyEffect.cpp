#include "HitEnemyEffect.h"
#include "debug.h"
CHitEnemyEffect::CHitEnemyEffect(float x, float y)
{
    float vx = 0.0f;    // tốc độ ngang
    float vy = 0.0f;   // tốc độ bay lên
    float ay = 0.0f;
    float lifetime = 0.15f;

    LPANIMATION anim = CAnimations::GetInstance()->Get(ID_HITENEMY_EFFECT);
    if (!anim)
    {
        DebugOutTitle(L"[ERROR] Brick break animation not found");
        return;
    }

    // Bay lên và lệch trái/phải, sau đó rơi
    AddParticle(new CParticle(x, y, -vx, -vy, ay, lifetime, anim)); // Trái trên
}
void CHitEnemyEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = 0;
    r = 0;
    b = 0;
}