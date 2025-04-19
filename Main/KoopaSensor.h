﻿#pragma once
#include "GameObject.h"
#define SENSOR_WIDTH 4
#define SENSOR_HEIGHT 4
class CKoopa; 

class CKoopaSensor : public CGameObject
{
protected:
    CKoopa *owner;
    bool hasTurned = false; 
public:
    CKoopaSensor(float x, float y) : CGameObject(x, y)
    {
        owner = nullptr;
    }
    
    void SetOwner(CKoopa* koopa) { owner = koopa; }

    virtual void GetBoundingBox(float& l, float& t, float& r, float& b)
    {
        l = x - SENSOR_WIDTH / 2;
        t = y - SENSOR_HEIGHT / 2;
        r = l + SENSOR_WIDTH;
        b = t + SENSOR_HEIGHT;
    }
    virtual int IsBlocking() { return 0; } // Không chặn gì cả
    int IsCollidable() override { return 0; }
    virtual void Render() { RenderBoundingBox(); }
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

