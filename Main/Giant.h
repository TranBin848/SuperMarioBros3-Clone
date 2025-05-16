#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_GIANT		13000
#define ID_ANI_GIANT_1UP	13001

#define	GIANT_WIDTH 15
#define GIANT_BBOX_WIDTH 16
#define GIANT_BBOX_HEIGHT 16

#define GIANT_STATE_IDLE 100
#define GIANT_STATE_ACTIVATE 200

#define GIANT_GRAVITY 0.001f
#define GIANT_WALKING_SPEED 0.06f
#define GIANT_BOUNCE_SPEED 0.02f

class CGiant : public CGameObject {
public:
	float ax;
	float ay;
	float originalY;
	float originalX;
	bool isActivating;
	int walkingDirection = -1;
	bool isHitByITB = false;
	bool isGreenGiant;
	CGiant(float x, float y, bool fl) : CGameObject(x, y) {
		originalY = y;
		originalX = x;
		this->ax = 0;
		this->ay = GIANT_GRAVITY;
		isActivating = false;
		isGreenGiant = fl;
		SetState(GIANT_STATE_IDLE);
	}
	void Render();
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void SetState(int state);
	void SetHitByITB() { isHitByITB = true; };
	void SetWalkingDirection(int dir) { walkingDirection = dir; }
	bool GetIsGreenGiant() { return isGreenGiant; };
};