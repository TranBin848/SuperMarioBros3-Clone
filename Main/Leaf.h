#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_LEAF_LEFT 13500
#define ID_ANI_LEAF_RIGHT 13501

#define	LEAF_WIDTH 15
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 13

#define LEAF_STATE_IDLE 100
#define LEAF_STATE_RISING 200
#define LEAF_STATE_FALLING 300
#define LEAF_STATE_GROUNDED 400

#define LEAF_GRAVITY 0.0003f
#define LEAF_RISE_SPEED 0.1f
#define LEAF_WALK_SPEED 0.05f
#define LEAF_MAX_HEIGHT 56.0f // Độ cao tối đa so với vị trí ban đầu
#define LEAF_SWAY_PERIOD 1000 // Chu kỳ dao động ngang (ms)
#define LEAF_FALL_MAX_SPEED 0.02f 

class CLeaf : public CGameObject {
public:
	float ax;
	float ay;
	float originalY;
	float originalX;
	float startY; // Vị trí y khi bắt đầu trạng thái FALLING
	ULONGLONG sway_start; // Thời điểm bắt đầu dao động ngang
	int walkingDirection = -1;
	CLeaf(float x, float y) : CGameObject(x, y) {
		originalX = x;
		originalY = y;
		startY = y; // Khởi tạo mặc định
		vx = 0;
		vy = 0;
		walkingDirection = -1; // Bắt đầu di chuyển sang trái
		sway_start = 0;
		SetState(LEAF_STATE_IDLE);
	}
	void Render();
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void SetState(int state);
	void SetWalkingDirection(int dir) { walkingDirection = dir; }
};