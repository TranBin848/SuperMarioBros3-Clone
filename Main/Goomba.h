#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.03f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_BBOX_HEIGHT_DIE 10

#define GOOMBA_DIE_TIMEOUT 1000

#define GOOMBA_STATE_WALKING	100
#define GOOMBA_STATE_DIE		200
#define GOOMBA_STATE_DIEBYSHELL	300

#define ID_ANI_GOOMBA_WALKING		5000
#define ID_ANI_GOOMBA_DIE			5001
#define ID_ANI_GOOMBA_DIEBYSHELL	5002

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);
};