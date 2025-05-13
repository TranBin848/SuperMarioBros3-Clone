#include "HUD.h"
#include"Game.h"
#include "debug.h"
CHUD* CHUD::__instance = NULL;
CHUD* CHUD::GetInstance()
{
    if (__instance == NULL)
        __instance = new CHUD(0, 0);
    return CHUD::__instance;
}
void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}
void CHUD::Render() {
	CGame* g = CGame::GetInstance();
	CSprites* s = CSprites::GetInstance();

	LPSPRITE sprite = s->Get(HUD_SPRITE_ID);
	if (!sprite)
		DebugOutTitle(L"khong co sprite: %f");
	else
		sprite->DrawStatic(x, y - HUD_HEIGHT / 2);
}