#pragma once
#include "GameObject.h"

class CMario;

class CEnemyActivator : public CGameObject
{
    bool isFront;
public:
    CEnemyActivator(bool fl) {
        isFront = fl;
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
        l = x; t = y - 350; r = x + 32; b = y + 350; // kích thước bbox trigger, tùy chỉnh
    }
    int IsBlocking() { return 0; };
    void Render(); // có thể không cần render gì cả
};

