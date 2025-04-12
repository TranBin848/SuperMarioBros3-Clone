#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_ITEMBOX_IDLE 12000

#define	ITEMBOX_WIDTH 15
#define ITEMBOX_BBOX_WIDTH 15
#define ITEMBOX_BBOX_HEIGHT 15

#define ITEMBOX_BOUNCE_HEIGHT 20  // Độ cao nảy lên
#define ITEMBOX_BOUNCE_SPEED 0.2f // Tốc độ nảy lên

#define ITEMBOX_STATE_IDLE 100
#define ITEMBOX_STATE_BOUNCING 200
#define ITEMBOX_STATE_ACTIVATE 300

#define ID_ANI_ITEMBOX_BOUNCING	12100

class CItemBox : public CGameObject {
protected:
	float originalY; // Lưu vị trí ban đầu
	bool pickable;
public:
	CItemBox(float x, float y) : CGameObject(x, y) {
		originalY = y;
		pickable = true;
		SetState(ITEMBOX_STATE_IDLE);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
};