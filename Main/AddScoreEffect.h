#pragma once
#include "ParticleSystem.h"

#define ID_ADDSCORE_EFFECT   21000

class CAddScoreEffect : public CParticleSystem
{
public:
    CAddScoreEffect(float x, float y, int score);
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};