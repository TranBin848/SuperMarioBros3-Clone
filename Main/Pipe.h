#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CPipe : public CGameObject {
protected:
	float cellWidth;
	float cellHeight;
	int spriteId;
	bool isHiddenPipe;
public:
	CPipe(float x, float y, float cell_width, float cell_height, int sprite_id, int fl) : CGameObject(x, y) {
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteId = sprite_id;
		if (fl == 1) isHiddenPipe = true;
		else isHiddenPipe = false;
		this->renderLayer = 5;
	}
	void Render();
	void Update(DWORD dt) {}
	void RenderBoundingBox();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
	bool GetIsHiddenPipe() { return isHiddenPipe; };
};