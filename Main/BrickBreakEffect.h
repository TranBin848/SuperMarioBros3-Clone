#pragma once
#include "ParticleSystem.h"

#define ID_BRICK_PIE    10011

class CBrickBreakEffect : public CParticleSystem
{
public:
    CBrickBreakEffect(float x, float y);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};