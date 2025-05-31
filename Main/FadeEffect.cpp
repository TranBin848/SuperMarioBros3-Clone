#include "FadeEffect.h"
#include "Game.h"
#include "Sprites.h"

CFadeEffect::CFadeEffect(int fadeState, float fadeSpeed)
{
    this->fadeState = fadeState;
    this->fadeSpeed = fadeSpeed;
    this->startTime = GetTickCount64();
    this->renderLayer = 20;
    if (fadeState == FADE_OUT_STATE)
        a = 1.0f; // fade out từ đen về trong
    else
        a = 0.0f; // fade in từ trong ra đen
}

void CFadeEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CFadeEffect::Render()
{
    if (fadeState == 0)
    {
        if (GetTickCount64() - startTime > FADE_EFFECT_TIME)
        {
            Delete(); // đánh dấu xóa object
            return;
        }
    }

    CSprites* s = CSprites::GetInstance();
    float dt = (GetTickCount64() - startTime);

    switch (fadeState)
    {
    case FADE_IN_STATE:
        a += fadeSpeed * dt;
        if (a >= 1.0f)
        {
            a = 1.0f;
            fadeState = 0; // dừng fade
        }
        break;

    case FADE_OUT_STATE:
        a -= fadeSpeed * dt;
        if (a <= 0.0f)
        {
            a = 0.0f;
            /*fadeState = 0;*/
        }
        break;
    }

    if (fadeState != 0)
        startTime = GetTickCount64(); // reset thời gian nếu đang fade

    s->Get(ID_SPRITE_BLACK_FILL)->DrawStaticFill(0, 0, a);
}
void CFadeEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = 0;
    r = 0;
    b = 0;
}