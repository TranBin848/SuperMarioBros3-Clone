#pragma once
#include "GameObject.h"


#define GOOMBA_GRAVITY 0.001f
#define GOOMBA_WALKING_SPEED 0.035f

#define GOOMBA_BBOX_WIDTH			16
#define GOOMBA_BBOX_HEIGHT_NORMAL	16
#define GOOMBA_BBOX_HEIGHT_NOWING	24
#define GOOMBA_BBOX_HEIGHT_WING		26
#define GOOMBA_BBOX_HEIGHT_DIE		10

#define GOOMBA_DIE_TIMEOUT 1000

#define PARAGOOMBA_STATE_WALKING_NOWING		100
#define PARAGOOMBA_STATE_WALKING_FLAPWING	200
#define PARAGOOMBA_STATE_FLY				300
#define PARAGOOMBA_STATE_NORMAL_WALKING		400
#define PARAGOOMBA_STATE_DIE				500
#define PARAGOOMBA_STATE_DIEBYSHELL			600

#define ID_ANI_PARAGOOMBA_WALKING			5100
#define ID_ANI_GOOMBA_WALK_FLAPWING_RIGHT	5101
#define ID_ANI_GOOMBA_WALK_FLAPWING_LEFT	5102
#define ID_ANI_GOOMBA_FLYRIGHT				5103
#define ID_ANI_GOOMBA_FLYLEFT				5104
#define ID_ANI_PARAGOOMBA_DIE				5105
#define ID_ANI_GOOMBA_NORMALWALK			5106
#define ID_ANI_PARAGOOMBA_DIEBYSHELL		5107


class CParaGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG nextStateTime; // thời điểm để đổi state tiếp theo
	ULONGLONG lastJumpTime;
	DWORD stateDelay;        // thời gian delay giữa các state

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CParaGoomba(float x, float y);
	virtual void SetState(int state);
};