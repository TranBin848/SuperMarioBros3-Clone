#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "ItemBoxSensor.h"
#include "PlayScene.h"
#define ID_ANI_ITEMBOX_IDLE			12000
#define ID_ANI_ITEMBOXBRICK_IDLE	10001

#define	ITEMBOX_WIDTH 15
#define ITEMBOX_BBOX_WIDTH 15
#define ITEMBOX_BBOX_HEIGHT 14

#define ITEMBOX_BOUNCE_HEIGHT 20  // Độ cao nảy lên
#define ITEMBOX_BOUNCE_SPEED 0.2f // Tốc độ nảy lên

#define ITEMBOX_STATE_IDLE 100
#define ITEMBOX_STATE_BOUNCING 200
#define ITEMBOX_STATE_ACTIVATE 300

#define ID_ANI_ITEMBOX_BOUNCING	12100

class CItemBox : public CGameObject {
protected:
	CItemBoxSensor* sensor;
	float originalY; // Lưu vị trí ban đầu
	bool pickable;
	int flagCoin;
	int bounceDirection = 1; // mặc định từ trái

public:
	CItemBox(float x, float y, int fl) : CGameObject(x, y) {
		sensor = new CItemBoxSensor(x, y);
		sensor->SetOwner(this);
		CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (scene)
		{
			scene->AddObject(sensor); // hoặc push vào vector<objects> tùy bạn tổ chức
		}
		flagCoin = fl;
		originalY = y;
		pickable = true;
		SetState(ITEMBOX_STATE_IDLE);
	}
	virtual int IsCollidable() {
		return 1;
	};
	virtual int IsBlocking() { return 1; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetBounceDirection(int dir) { bounceDirection = dir; }
	int GetBounceDirection() const { return bounceDirection; }
};