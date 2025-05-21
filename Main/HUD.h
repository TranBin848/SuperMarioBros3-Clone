#pragma once

#include "GameObject.h"

#define HUD_SPRITE_ID	65000
#define HUD_HEIGHT	47
class CHUD : public CGameObject
{
protected:
	static CHUD* __instance;
	int score = 0;
	int coin = 0;
	int timeLeft = 300;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void RenderNumber(int number, int digits, float x, float y); // Helper để vẽ số

public:
	CHUD(float x, float y) :CGameObject(x, y) {}	
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	static CHUD* GetInstance();

	void SetScore(int value) { score = value; }
	void SetCoin(int value) { coin = value; }
	void SetTime(int value) { timeLeft = value; }
};

