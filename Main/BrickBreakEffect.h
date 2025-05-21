#pragma once
#include "ParticleSystem.h"

#define ID_BRICK_PIE    21001

class CBrickBreakEffect : public CParticleSystem
{
public:
    CBrickBreakEffect(float x, float y);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};