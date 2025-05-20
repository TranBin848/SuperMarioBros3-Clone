#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

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
		this->renderLayer = 5;
	}
	void Render();
	void Update(DWORD dt) {}
	void RenderBoundingBox();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
};