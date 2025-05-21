#pragma once

#include "GameObject.h"
#include "Animation.h" 

class CParticle : public CGameObject
{
protected:
    float vx, vy, ay;
    float lifetime;
    float age = 0;
    LPANIMATION animation; 
public:
    CParticle(float x, float y, float vx, float vy, float lifetime, float ay, LPANIMATION animation);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    bool IsDead() const { return age >= lifetime; }
};