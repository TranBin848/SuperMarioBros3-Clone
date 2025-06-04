#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_FIRE 14100

#define	FIRE_WIDTH 7
#define FIRE_BBOX_WIDTH 7
#define FIRE_BBOX_HEIGHT 7

#define FIRE_SPEED 0.08f

#define FIRE_LIFETIME 4000 // 4 giây


class CFire : public CGameObject {
protected:
	ULONGLONG fire_start = 0;
public:
	float originalY;

	CFire(float x, float y) : CGameObject(x, y) {
		fire_start = GetTickCount64(); 
	};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
};

