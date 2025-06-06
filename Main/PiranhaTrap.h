#pragma once
#include "GameObject.h"

#define VENUSFIRE_ARISE_SPEED 0.05f
#define VENUSFIRE_WAIT_TIME 2000 // thời gian chờ giữa lên và xuống (ms)
#define VENUSFIRE_TOP_WAIT 500 // 0.5 giây


#define VENUSFIRE_BBOX_WIDTH 15
#define VENUSFIRE_BBOX_HEIGHT 19
#define VENUSFIRE_HEIGHT 27

#define ID_ANI_PIRANHA 14016

class CPiranhaTrap : public CGameObject
{
protected:
	ULONGLONG wait_start = 0;
	ULONGLONG top_start = 0;
	bool isWaiting = false;
	float originalY;
	bool isGoingUp = true; // cây đang trồi lên hay hạ xuống
	bool isAtTop = false;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPiranhaTrap(float x, float y) : CGameObject(x, y) {
		originalY = y;
	};
	bool GetIsWaiting() { return y == originalY; };
};