#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	if (mario->GetState() == MARIO_STATE_ENTER_PIPE || mario->GetState() == MARIO_STATE_EXIT_PIPE)
		return;
	switch (KeyCode)
	{
	case DIK_DOWN:
	{
		bool fl = mario->GetIsOnHiddenPipe();
		if (!fl)
			mario->SetState(MARIO_STATE_SIT);
		else
		{
			mario->SetState(MARIO_STATE_ENTER_PIPE);
		}
		break;
	}
	case DIK_S:
	{
		bool fl = mario->GetIsOnHiddenPipe();
		int currentScene = CGame::GetInstance()->GetCurrentSceneId();
		if (!fl)
		{
			if (!isJumpPressed)
			{
				mario->SetState(MARIO_STATE_JUMP);
				isJumpPressed = true;
			}
		}	
		else
		{
			if(currentScene == 1)
				mario->SetState(MARIO_STATE_ENTER_PIPE);
			else 
				if (!isJumpPressed)
				{
					mario->SetState(MARIO_STATE_JUMP);
					isJumpPressed = true;
				}
		}
		break;
	}
	case DIK_X:
	{
		if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
			mario->SetState(TANUKI_STATE_TAILATTACK);
		break;
	}
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TANUKI);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_TANUKI)
			mario->SetState(TANUKI_STATE_TAILATTACK);
		break;
	case DIK_F1:
		mario->SetPosition(2700, 120);
		break;
	case DIK_F2:
		mario->SetPosition(555, 120);
		break;
	case DIK_R: // reset
		CGame::GetInstance()->ReloadScene();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_ENTER_PIPE || mario->GetState() == MARIO_STATE_EXIT_PIPE)
		return;
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetState() != MARIO_STATE_ENTER_PIPE)
		{
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
			isJumpPressed = false;
		}
		break;
	case DIK_DOWN:
		if(mario->GetState() != MARIO_STATE_ENTER_PIPE)
			mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetState() == MARIO_STATE_ENTER_PIPE || mario -> GetState() == MARIO_STATE_EXIT_PIPE)
		return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
	{
		mario->SetState(MARIO_STATE_IDLE);
	}	
}