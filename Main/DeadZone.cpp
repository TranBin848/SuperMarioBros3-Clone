#include "DeadZone.h"

void CDeadZone::Render()
{
	RenderBoundingBox();
}

void CDeadZone::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - DEADZONE_BBOX_WIDTH / 2;
	t = y - DEADZONE_BBOX_HEIGHT / 2;
	r = l + DEADZONE_BBOX_WIDTH;
	b = t + DEADZONE_BBOX_HEIGHT;
}