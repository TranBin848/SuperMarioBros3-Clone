#pragma once
#include "Particle.h"
#include <vector>

class CParticleSystem : public CGameObject
{
protected:
    std::vector<CParticle*> particles;

public:
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void AddParticle(CParticle* p);

    bool IsEmpty() const { return particles.empty(); }
};