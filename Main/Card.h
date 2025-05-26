#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_CARD_ACTIVATE 30000


#define CARD_STATE_IDLE	100
#define CARD_STATE_ACTIVATE	200
#define CARD_BBOX_WIDTH	15
#define CARD_BBOX_HEIGHT 15

#define CARD_BOUNCE_SPEED 0.15f
#define CARD_BOUNCE_HEIGHT  8.0f

#define CARD_SWITCH_TIME    200 

//BOUNCING
#define ID_ANI_CARD_STAR		30001
#define ID_ANI_CARD_MUSHROOM	30002
#define ID_ANI_CARD_FLOWER		30003
//IDLE
#define ID_ANI_CARD_STAR_IDLE		30004
#define ID_ANI_CARD_MUSHROOM_IDLE	30005
#define ID_ANI_CARD_FLOWER_IDLE		30006

enum CardType {
	CARD_TYPE_MUSHROOM = 0,
	CARD_TYPE_FLOWER = 1,
	CARD_TYPE_STAR = 2
};

class CCard : public CGameObject {
protected:
	int state;
	CardType currentType = CARD_TYPE_MUSHROOM;
	float originalY;
	float bounceVy = -CARD_BOUNCE_SPEED;
	DWORD switching_start = 0;
	DWORD switching_interval = 200; // ms, đổi mỗi 0.2s

	bool isActivated = false;

public:
	CCard(float x, float y);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	int IsBlocking() override { return 0; }

	void SetState(int state);
	bool IsActivated() { return isActivated; }
	void Activate();

	CardType GetCardType() { return currentType; }
};