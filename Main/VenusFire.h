#pragma once
#include "GameObject.h"

#define VENUSFIRE_ARISE_SPEED 0.05f
#define VENUSFIRE_WAIT_TIME 2000 // thời gian chờ giữa lên và xuống (ms)
#define VENUSFIRE_TOP_WAIT 500 // 0.5 giây


#define VENUSFIRE_BBOX_WIDTH 15
#define VENUSFIRE_BBOX_HEIGHT 31

#define VENUSFIRE_STATE_ARISE 100
#define VENUSFIRE_STATE_SHOOT 200

#define ID_ANI_ARISE_UP 14000
#define ID_ANI_ARISE_DOWN 14001
#define ID_ANI_SHOOT_UP 14002
#define ID_ANI_SHOOT_DOWN 14003
const float FIXED_ANGLES_RIGHT[] = { 50.0f, 66.0f, 82.0f, 98.0f, 114.0f, 130.0f };
const float FIXED_ANGLES_LEFT[] = { 310.0f, 294.0f, 278.0f, 262.0f, 246.0f, 230.0f };
const int NUM_ANGLES = sizeof(FIXED_ANGLES_RIGHT) / sizeof(FIXED_ANGLES_RIGHT[0]);

class CVenusFire : public CGameObject
{
protected:
	ULONGLONG wait_start = 0;
	ULONGLONG top_start = 0;
	bool isWaiting = false;
	float originalY;
	bool isGoingUp = true; // cây đang trồi lên hay hạ xuống
	bool hasShot = false; // đã bắn chưa?
	bool isAtTop = false;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CVenusFire(float x, float y) : CGameObject(x, y) {
		originalY = y; // ← Thêm dòng này
		state = VENUSFIRE_STATE_ARISE;
	};
};