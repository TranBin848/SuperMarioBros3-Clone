#pragma once

#include "GameObject.h"

#define ID_ANI_HUD	20001	
#define HUD_SPRITE_ID	65000
#define HUD_HEIGHT	47
class CHUD : public CGameObject
{
protected:
	static CHUD* __instance;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
	CHUD(float x, float y) :CGameObject(x, y) {}	
	void Render();
	static CHUD* GetInstance();
};

