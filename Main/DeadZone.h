#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define DEADZONE_WIDTH 16
#define DEADZONE_BBOX_WIDTH 55
#define DEADZONE_BBOX_HEIGHT 50

class CDeadZone : public CGameObject {
protected:
public:
	CDeadZone(float x, float y) : CGameObject(x, y) {}
	void Render();
	virtual int IsBlocking() { return 1; }
	virtual int IsCollidable() { return 1; }
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};