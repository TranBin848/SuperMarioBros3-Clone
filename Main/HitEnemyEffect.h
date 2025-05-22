#pragma once
#include "ParticleSystem.h"

#define ID_HITENEMY_EFFECT   21003

class CHitEnemyEffect : public CParticleSystem
{
public:
    CHitEnemyEffect(float x, float y);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};