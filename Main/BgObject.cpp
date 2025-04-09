#include "BgObject.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"

void CBgObject::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);


}

void CBgObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	
}