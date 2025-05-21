#pragma once
#include "ParticleSystem.h"

#define ID_BRICK_PIE    21001

class CSwitchBlockEffect : public CParticleSystem
{
public:
    CSwitchBlockEffect(float x, float y);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};