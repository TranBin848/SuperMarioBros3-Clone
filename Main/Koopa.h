#pragma once
#include "GameObject.h"
#include "KoopaSensor.h"
#include "PlayScene.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_ACTIVATE_SPEED 0.2f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 24
#define KOOPA_BBOX_HEIGHT_SHELL	14

#define KOOPA_SHELL_TIMEOUT 4000

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_SHELL 200
#define KOOPA_STATE_ACTIVATE 300
#define KOOPA_STATE_RETURN 400

#define ID_ANI_KOOPA_WALKING 6000
#define ID_ANI_KOOPA_SHELL 6001
#define ID_ANI_KOOPA_ACTIVATE 6002
#define ID_ANI_KOOPA_RETURN 6003

class CKoopa : public CGameObject
{
protected:
	CKoopaSensor* sensor;

	float ax;
	float ay;
	bool just_activated; // Thêm biến này
	bool isBeingHeld = false;
	ULONGLONG die_start;
	ULONGLONG shell_start;
	ULONGLONG return_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return !isBeingHeld ? 1 : 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithItemBox(LPCOLLISIONEVENT e);
public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
	virtual void SetVX() { vx = -vx; };
	void SetIsBeingHeld(bool held) { isBeingHeld = held; }
};