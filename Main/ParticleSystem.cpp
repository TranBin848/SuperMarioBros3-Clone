#include "ParticleSystem.h"
#include "debug.h"
void CParticleSystem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i]->Update(dt, coObjects);
    }

    // Xóa particle đã chết
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](CParticle* p) {
            bool dead = p->IsDead();
            if (dead) delete p;
            return dead;
        }),
        particles.end());
}

void CParticleSystem::Render()
{
    for (auto p : particles)
    {
        
        p->Render();
    }
}

void CParticleSystem::AddParticle(CParticle* p)
{
    particles.push_back(p);
}