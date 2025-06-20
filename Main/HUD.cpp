﻿#include "HUD.h"
#include"Game.h"
#include "debug.h"
#include "Mario.h"
CHUD* CHUD::__instance = NULL;
CHUD* CHUD::GetInstance()
{
    if (__instance == NULL)
        __instance = new CHUD(0, 0);
    return CHUD::__instance;
}
void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	timeAccumulator += dt / 1000.0f; 
	level = CMario::GetInstance()->GetLevel();
	if (CMario::GetInstance()->GetAtEndMap())
	{
		if (endMapStartTime == 0)
			endMapStartTime = GetTickCount64();
		level = 1;
	}
	else
	{
		endMapStartTime = 0;
		isConvertingTimeToScore = false;
	}

	if (timeAccumulator >= 1.0f)
	{
		timeAccumulator -= 1.0f;
		if (timeLeft > 0)
			timeLeft--;
	}
	if (isConvertingTimeToScore && timeLeft > 0)
	{
		// Trừ nhanh mỗi 30ms
		if (GetTickCount64() - timeConversionTimer >= 0.1f)
		{
			timeLeft--;
			score += 50;
			timeConversionTimer = GetTickCount64();
		}
	}
	// Blink logic cho max power bar
	if (CMario::GetInstance()->GetMaxPower() || currentCard != -1)
	{
		blinkAccumulator += dt;
		cardBlinkAccumulator += dt;
		if (blinkAccumulator >= 200) // nháy mỗi 200ms
		{
			blinkVisible = !blinkVisible;
			blinkAccumulator = 0;
		}
		if (cardBlinkAccumulator >= 400)
		{
			cardBlickInvisible = !cardBlickInvisible;
			cardBlinkAccumulator = 0;
		}
	}
	else
	{
		// Reset nếu không còn max power
		blinkVisible = true;
		blinkAccumulator = 0;
	}
	/*DebugOutTitle(L"timeLeft: %d", timeLeft);*/
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
	RenderRunPower(CMario::GetInstance()->GetRunPower(), x - 64, y - 35);
	if (CMario::GetInstance()->GetAtEndMap())
	{
		CSprites* s = CSprites::GetInstance();

		// Vẽ "COURSE CLEAR" ngay lập tức
		LPSPRITE textEnd = s->Get(ID_HUD_TEXTEND);
		if (textEnd)
			textEnd->DrawStatic(x + 12, y - 200);

		// Sau 1 giây mới vẽ "YOU GOT A CARD"
		if (GetTickCount64() - endMapStartTime >= 1000)
		{
			LPSPRITE textCard = s->Get(ID_HUD_TEXTCARD);
			if (textCard)
				textCard->DrawStatic(x + 12, y - 170);
			if (currentCard != -1)
			{
				int cardSpriteId = -1;

				switch (currentCard)
				{
					/*DebugOutTitle(L"id: %d", currentCard);*/
					case 0: cardSpriteId = ID_HUD_STARCARD; break;
					case 1: cardSpriteId = ID_HUD_GIANTCARD; break;
					case 2: cardSpriteId = ID_HUD_FLOWERCARD; break;
				}

				LPSPRITE cardSprite = s->Get(cardSpriteId);
				if (cardSprite)
				{
					cardSprite->DrawStatic(x + 84, y - 170);
					if(cardBlickInvisible)
						cardSprite->DrawStatic(x + 60, y - 31);
				}
					
			}
			isConvertingTimeToScore = true;
		}
	}
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
void CHUD::Reset()
{
	score = 0;
	coin = 0;
	timeLeft = 300;
	life = 4;
	currentCard = -1;
	CMario::GetInstance()->SetLevel(1);
}
void CHUD::RenderRunPower(int power, float x, float y)
{
	CSprites* s = CSprites::GetInstance();

	for (int i = 0; i < 6; i++)
	{
		int spriteId = (i < power) ? ID_HUD_RUNBAR_FILLED : ID_HUD_RUNBAR_EMPTY;
		LPSPRITE bar = s->Get(spriteId);
		if (bar)
			bar->DrawStatic(x + i * 8, y);  // Mỗi nấc cách nhau 8px
	}
	// Vẽ nấc 7 (max power)
	int spriteIdLast = ID_HUD_RUNBAR_BLINK_OFF;  // mặc định luôn vẽ sprite tĩnh "tối"
	if (CMario::GetInstance()->GetMaxPower())
	{
		// Khi max power thì nhấp nháy
		spriteIdLast = blinkVisible ? ID_HUD_RUNBAR_BLINK_ON : ID_HUD_RUNBAR_BLINK_OFF;
	}

	LPSPRITE barLast = s->Get(spriteIdLast);
	if (barLast)
		barLast->DrawStatic(x + 6 * 8 + 5, y);
}