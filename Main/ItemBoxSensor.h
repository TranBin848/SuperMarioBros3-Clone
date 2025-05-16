#pragma once
#include "GameObject.h"
#include "debug.h"
#define SENSOR_WIDTH 4
#define SENSOR_HEIGHT 4
class CItemBox;

class CItemBoxSensor : public CGameObject
{
protected:
    CItemBox* owner;
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
    void OnCollisionWithGiant(LPCOLLISIONEVENT e);
public:
    CItemBoxSensor(float x, float y) : CGameObject(x, y - 16.0f)
    {
        owner = nullptr;
    }

    void SetOwner(CItemBox* itb) { owner = itb; }

    virtual void GetBoundingBox(float& l, float& t, float& r, float& b)
    {
        l = x - SENSOR_WIDTH / 2;
        t = y - SENSOR_HEIGHT / 2;
        r = l + SENSOR_WIDTH;
        b = t + SENSOR_HEIGHT;
    }
    virtual int IsBlocking() { return 0; } // Không chặn gì cả
    int IsCollidable() override { return 1; }
    virtual void Render() { RenderBoundingBox(); }
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

