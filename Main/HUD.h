#pragma once

#include "GameObject.h"

#define HUD_SPRITE_ID	65000
#define HUD_HEIGHT	47
#define ID_HUD_RUNBAR_EMPTY 65200
#define ID_HUD_RUNBAR_FILLED 65201
#define ID_HUD_RUNBAR_BLINK_ON 65202
#define ID_HUD_RUNBAR_BLINK_OFF 65203
class CHUD : public CGameObject
{
protected:
	static CHUD* __instance;
	int score = 0;
	int coin = 0;
	int timeLeft = 300;
	int life = 4;
	float timeAccumulator = 0.0f;
	bool blinkVisible = true;
	float blinkAccumulator = 0.0f;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	void RenderNumber(int number, int digits, float x, float y); // Helper để vẽ số
	void RenderRunPower(int power, float x, float y); 

public:
	CHUD(float x, float y) :CGameObject(x, y) {}
	void Render();
	static CHUD* GetInstance();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetScore(int value) { score += value; }
	void SetCoin(int value) { coin += value; }
	void SetTime(int value) { timeLeft = value; }
	void SetLife(int value) { life += value; };
};

