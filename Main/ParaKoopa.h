#pragma once
#include "GameObject.h"
#include "PlayScene.h"
#define PARAKOOPA_GRAVITY 0.0005f
#define PARAKOOPA_JUMP_SPEED	0.2f
#define PARAKOOPA_WALKING_SPEED 0.05f
#define PARAKOOPA_ACTIVATE_SPEED 0.2f

#define PARAKOOPA_BBOX_WIDTH		16
#define PARAKOOPA_BBOX_HEIGHT		26
#define KOOPA_BBOX_HEIGHT			24
#define PARAKOOPA_BBOX_HEIGHT_SHELL	14

#define PARAKOOPA_SHELL_TIMEOUT 4000

#define PARAKOOPA_STATE_WALKING		100
#define	PARAKOOPA_STATE_NORMAL_WALKING	101
#define PARAKOOPA_STATE_SHELL		200
#define PARAKOOPA_STATE_ACTIVATE	300
#define PARAKOOPA_STATE_RETURN		400

#define ID_ANI_GRKOOPA_WALKING		6500
#define ID_ANI_GRKOOPA_SHELL		6501
#define ID_ANI_GRKOOPA_ACTIVATE		6502
#define ID_ANI_GRKOOPA_RETURN		6503
#define ID_ANI_PARAKOOPA_WALKING	5200

class CParaKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	bool just_activated; 
	bool isOnGround = false;
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
	CParaKoopa(float x, float y);
	virtual void SetState(int state);
	virtual void SetVX() { vx = -vx; };
	void SetIsBeingHeld(bool held) { isBeingHeld = held; }
};