#pragma once
#include "GameObject.h"

class CMario;

class CEnemyActivator : public CGameObject
{
public:
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
        l = x; t = y; r = x + 32; b = y + 350; // kích thước bbox trigger, tùy chỉnh
    }
    int IsBlocking() { return 0; };
    void Render(); // có thể không cần render gì cả
};

