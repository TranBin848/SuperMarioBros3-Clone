#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000
#define ID_ANI_COIN_ACTIVATE 11100
#define ID_ANI_COIN_BOUNCING 11100

#define COIN_STATE_IDLE	100
#define COIN_STATE_ACTIVATE	200
#define COIN_STATE_BOUNCING 300
#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_BOUNCE_SPEED 0.2f
#define COIN_GRAVITY 0.15f


class CCoin : public CGameObject {
public:
	float originalY;

	CCoin(float x, float y) : CGameObject(x, y) {
		originalY = y;
		SetState(COIN_STATE_IDLE);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};