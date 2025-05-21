#include "Particle.h"
#include "debug.h"
CParticle::CParticle(float x, float y, float vx, float vy, float ay, float lifetime, LPANIMATION animation)
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->ay = ay;
    this->lifetime = lifetime;
    this->animation = animation;
}

void CParticle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    float dtSec = dt / 1000.0f;
    x += vx * dtSec;
    y += vy * dtSec;

    vy += ay * dtSec; // gravity

    age += dtSec;
}

void CParticle::Render()
{
    if (animation)
    {
        animation->Render(x, y);
    }
    
}
void CParticle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = 0;
    r = 0;
    b = 0;
}