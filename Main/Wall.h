#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define WALL_WIDTH 16
#define WALL_BBOX_WIDTH 16
#define WALL_BBOX_HEIGHT 450

class CWall : public CGameObject {
protected:
public:
	CWall(float x, float y) : CGameObject(x, y) {}
	void Render();
	virtual int IsBlocking() { return 1; }
	virtual int IsCollidable() { return 1; }
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};