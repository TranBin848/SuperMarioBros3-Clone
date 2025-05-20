#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10001
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 15

class CBrick : public CGameObject {
protected:
	bool canRemove;
public:
	CBrick(float x, float y, int fl) : CGameObject(x, y) {
		if (fl == 1) canRemove = true;
		else canRemove = false;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool GetCanRemove() { return canRemove; };
};