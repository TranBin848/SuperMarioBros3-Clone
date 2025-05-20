#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_SWITCHBLOCK			10002
#define ID_ANI_SWITCHBLOCK_ACTIVATED	10003
#define	SWITCHBLOCK_WIDTH 15
#define SWITCHBLOCK_BBOX_WIDTH 15
#define SWITCHBLOCK_BBOX_HEIGHT 15
#define SWITCHBLOCK_BBOX_HEIGHT_ACTIVATED 7
#define SWITCHBLOCK_STATE_IDLE 100
#define SWITCHBLOCK_STATE_ACTIVATED 300

class CSwitchBlock : public CGameObject {
protected:
	float originalY; // Lưu vị trí ban đầu
	int currState;
	bool finishRemoveBrick;
public:
	CSwitchBlock(float x, float y) : CGameObject(x, y) {
		originalY = y;
		finishRemoveBrick = false;
		SetState(SWITCHBLOCK_STATE_IDLE);
	}
	virtual int IsCollidable() {
		return 1;
	};
	virtual int IsBlocking() { return 1; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

};