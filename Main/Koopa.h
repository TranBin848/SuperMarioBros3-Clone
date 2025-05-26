#pragma once
#include "GameObject.h"
#include "KoopaSensor.h"
#include "PlayScene.h"	

#define KOOPA_GRAVITY				0.001f
#define PARAKOOPA_GRAVITY			0.0008f
#define KOOPA_WALKING_SPEED			-0.03f
#define KOOPA_ACTIVATE_SPEED		0.2f

#define PARAKOOPA_GRAVITY			0.0005f
#define PARAKOOPA_JUMP_SPEED		0.2f
#define KOOPA_JUMP_SPEED			0.3f

#define PARAKOOPA_BBOX_HEIGHT		28
#define KOOPA_BBOX_WIDTH			16
#define KOOPA_BBOX_HEIGHT			28
#define KOOPA_BBOX_HEIGHT_SHELL		16

#define KOOPA_SHELL_TIMEOUT			6000
#define KOOPA_SHELL_RECOVER			1000
#define KOOPA_DIE_TIMEOUT			1000

#define KOOPA_STATE_WALKING			100
#define KOOPA_STATE_SHELL			200	
#define KOOPA_STATE_ACTIVATE		300
#define KOOPA_STATE_RETURN			400
#define KOOPA_STATE_DIEBYSHELL		500

//RED
#define ID_ANI_KOOPA_WALKING		6000
#define ID_ANI_KOOPA_SHELL			6001
#define ID_ANI_KOOPA_ACTIVATE		6002
#define ID_ANI_KOOPA_RETURN			6003
#define ID_ANI_KOOPA_DIEBYSHELL		6005
//GREEN
#define ID_ANI_GRKOOPA_WALKING		6500
#define ID_ANI_GRKOOPA_SHELL		6501
#define ID_ANI_GRKOOPA_ACTIVATE		6502
#define ID_ANI_GRKOOPA_RETURN		6503
#define ID_ANI_GRKOOPA_DIEBYSHELL	6505

//PARA
#define ID_ANI_PARAKOOPA_WALKING	5200

#define	KOOPA_LEVEL_RED				1
#define	KOOPA_LEVEL_GREEN			2
#define KOOPA_LEVEL_PARA			3

#define SHELL_OFFSET_X				12.0f
#define SHELL_OFFSET_Y				1.0f

class CKoopa : public CGameObject
{
protected:
	CKoopaSensor* sensor;

	float ax;
	float ay;

	int level;

	bool just_activated; // Thêm biến này
	bool isOnGround = false;
	bool isBeingHeld = false;
	bool isUpsideDown = false;
	bool isHitByTail = false;
	int dir;
	ULONGLONG die_start;
	ULONGLONG shell_start;
	ULONGLONG return_start;

	int GetAniIdRed();
	int GetAniIdGreen();
	int GetAniIdPara();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() {
		if (isBeingHeld)
			return 0;
		return (state != KOOPA_STATE_DIEBYSHELL);
	};
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithItemBox(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithShinyBrick(LPCOLLISIONEVENT e);
public:
	CKoopa(float x, float y, int flag);
	virtual void SetState(int state);
	void SetLevel(int l);
	int GetLevel() { return level; };
	void GetAcc(float& ax, float& ay) { ax = this->ax; ay = this->ay; };
	virtual void SetVX() { vx = -vx; };
	void SetIsBeingHeld(bool held) { isBeingHeld = held; }
	bool GetIsBeingHeld() { return isBeingHeld; }
	void SetIsUpsideDown(bool fl) { isUpsideDown = fl; }
	bool GetIsUpsideDown() { return isUpsideDown; }
	void SetIsHitByTail(bool fl, int direction) { isHitByTail = fl; dir = direction; }
	bool GetIsHitByTail() { return isHitByTail; }
};