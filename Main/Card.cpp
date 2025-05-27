#include "Card.h"
#include "Animations.h"
#include "Game.h"
#include "Mario.h"
CCard::CCard(float x, float y) : CGameObject(x, y) {
	originalY = y;
	switching_start = GetTickCount64();
	SetState(CARD_STATE_IDLE);
}

void CCard::SetState(int state) {
	this->state = state;

	switch (state) {
	case CARD_STATE_IDLE:
		vx = vy = 0;
		break;
	case CARD_STATE_ACTIVATE:
		isActivated = true;
		vy = bounceVy;
		break;
	}
}

void CCard::Activate() {
	SetState(CARD_STATE_ACTIVATE);
}

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);

	if (state == CARD_STATE_IDLE) {
		if (GetTickCount64() - switching_start > switching_interval) {
			currentType = (CardType)((currentType + 1) % 3); 
			switching_start = GetTickCount64();
		}
	}
	else if (state == CARD_STATE_ACTIVATE) {
		y += vy * dt;

		// Khi bay lên cao khoảng 48px thì dừng lại
		if (originalY - y > 108.0f) {
			vy = 0;
			this->isDeleted = true;
		}
	}
}

void CCard::Render() {
	int aniId = -1;

	if (state == CARD_STATE_IDLE) {
		switch (currentType) {
		case CARD_TYPE_MUSHROOM: aniId = ID_ANI_CARD_MUSHROOM_IDLE; break;
		case CARD_TYPE_FLOWER: aniId = ID_ANI_CARD_FLOWER_IDLE; break;
		case CARD_TYPE_STAR: aniId = ID_ANI_CARD_STAR_IDLE; break;
		}
	}
	else if (state == CARD_STATE_ACTIVATE) {
		switch (currentType) {
		case CARD_TYPE_MUSHROOM: aniId = ID_ANI_CARD_MUSHROOM; break;
		case CARD_TYPE_FLOWER: aniId = ID_ANI_CARD_FLOWER; break;
		case CARD_TYPE_STAR: aniId = ID_ANI_CARD_STAR; break;
		}
	}

	if (aniId != -1) {
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}

	// Optional: RenderBoundingBox();
}

void CCard::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - CARD_BBOX_WIDTH / 2;
	t = y - CARD_BBOX_HEIGHT / 2;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_HEIGHT;
}