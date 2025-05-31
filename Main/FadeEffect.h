#pragma once
#include "ParticleSystem.h"
#define ID_SPRITE_BLACK_FILL    60024
#define FADE_OUT_STATE  2
#define FADE_IN_STATE   1
#define FADE_EFFECT_TIME	1000
class CFadeEffect : public CParticleSystem
{
    int fadeState;
    float a = 0.0f;
    float fadeSpeed;
    ULONGLONG startTime;

public:
    CFadeEffect(int fadeState, float fadeSpeed = 0.0005f);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    bool IsEmpty() const override { return fadeState == 0;}
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};

