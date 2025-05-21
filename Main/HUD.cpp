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
void CHUD::Render()
{
	CGame* g = CGame::GetInstance();
	CSprites* s = CSprites::GetInstance();

	// Vẽ nền HUD
	LPSPRITE sprite = s->Get(HUD_SPRITE_ID);
	if (sprite)
		sprite->DrawStatic(x, y - HUD_HEIGHT / 2);

	// Vị trí các số (tuỳ bạn điều chỉnh)
	RenderNumber(score, 6, x + 30, y + 5);     // score: 6 chữ số
	RenderNumber(coin, 2, x + 130, y + 5);     // coin: 2 chữ số
	RenderNumber(timeLeft, 3, x + 200, y + 5); // time: 3 chữ số
}
void CHUD::RenderNumber(int number, int digits, float x, float y)
{
	string str = to_string(number);
	while (str.length() < digits)
		str = "0" + str;

	CSprites* s = CSprites::GetInstance();

	for (int i = 0; i < digits; i++)
	{
		char c = str[i];
		int digit = c - '0';
		LPSPRITE sprite = s->Get(60000 + digit); // Ví dụ sprite số 0 có ID là 60000, 1 là 60001, ...
		if (sprite)
			sprite->DrawStatic(x + i * 8, y); // Khoảng cách mỗi số là 8px
	}
}