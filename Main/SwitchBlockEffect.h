#pragma once
#include "ParticleSystem.h"

#define ID_SB_SMOKEEFFECT    21002

class CSwitchBlockEffect : public CParticleSystem
{
public:
    CSwitchBlockEffect(float x, float y);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};