#pragma once

#include "GameObject.h"

#define HUD_SPRITE_ID	65000
#define HUD_HEIGHT	47
#define ID_HUD_RUNBAR_EMPTY 65200
#define ID_HUD_RUNBAR_FILLED 65201
#define ID_HUD_RUNBAR_BLINK_ON 65202
#define ID_HUD_RUNBAR_BLINK_OFF 65203
#define ID_HUD_TEXTEND			65204
#define ID_HUD_TEXTCARD			65205
#define ID_HUD_STARCARD			65206
#define ID_HUD_GIANTCARD		65207
#define ID_HUD_FLOWERCARD		65208
class CHUD : public CGameObject
{
protected:
	ULONGLONG endMapStartTime = 0;
	static CHUD* __instance;
	int score = 0;
	int coin = 0;
	int timeLeft = 300;
	int life = 4;
	int currentCard = -1; 
	int level;
	float timeAccumulator = 0.0f;
	bool blinkVisible = true;
	bool cardBlickInvisible = true;
	float blinkAccumulator = 0.0f;
	float cardBlinkAccumulator = 0.0f;
	bool isConvertingTimeToScore = false;
	ULONGLONG timeConversionTimer = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }

	void RenderNumber(int number, int digits, float x, float y); // Helper để vẽ số
	void RenderRunPower(int power, float x, float y); 

public:
	CHUD(float x, float y) :CGameObject(x, y) {
		level = 1;
	}
	void Render();
	static CHUD* GetInstance();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Reset();
	void SetScore(int value) { score += value; }
	void SetCoin(int value) { coin += value; }
	void SetTime(int value) { timeLeft = value; }
	void SetLife(int value) { life += value; };
	void SetCard(int value) { currentCard = value; }
	int GetLevel() { return level; }
	void SetLevel(int l) { level = l; };
};

