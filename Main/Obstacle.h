#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CObstacle : public CGameObject {
protected:
	float cellWidth;
	float cellHeight;
	int spriteId;
public:
	CObstacle(float x, float y, float cell_width, float cell_height, int sprite_id) : CGameObject(x, y) {
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteId = sprite_id;
	}
	void Render();
	void Update(DWORD dt) {}
	void RenderBoundingBox();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
};