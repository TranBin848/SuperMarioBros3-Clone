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
	timeAccumulator += dt / 1000.0f; 

	if (timeAccumulator >= 1.0f)
	{
		timeAccumulator -= 1.0f;
		if (timeLeft > 0)
			timeLeft--;
	}
	DebugOutTitle(L"timeLeft: %d", timeLeft);
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
	RenderNumber(score, 7, x - 64, y - 27);     // score: 6 chữ số
	RenderNumber(coin, 2, x + 16, y -35);     // coin: 2 chữ số
	RenderNumber(timeLeft, 3, x + 8, y - 27); // time: 3 chữ số
	RenderNumber(life, 2, x - 87, y - 27);     // score: 6 chữ số
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
		
		LPSPRITE sprite = s->Get(65100 + digit); 
		if (sprite)
		{
			sprite->DrawStatic(x + i * 8, y); // Khoảng cách mỗi số là 8px
			
		}
	}
}