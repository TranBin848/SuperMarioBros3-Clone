#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_FIRE 14100

#define	FIRE_WIDTH 7
#define FIRE_BBOX_WIDTH 7
#define FIRE_BBOX_HEIGHT 7

#define FIRE_SPEED 0.08f

class CFire : public CGameObject {
public:
	float originalY;

	CFire(float x, float y) : CGameObject(x, y) {};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; }
};

