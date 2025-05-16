#pragma once
#include "GameObject.h"
#include "Koopa.h"

#include "EnemyActivator.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define MARIO_WALKING_SPEED				0.1f
#define MARIO_RUNNING_SPEED				0.15f
#define MARIO_RUNNING_MAXPOWER_SPEED	0.2f

#define MARIO_ACCEL_WALK_X	0.0004f
#define MARIO_ACCEL_RUN_X	0.0006f
#define MARIO_DECEL_WALK_X 0.00015f

#define MARIO_JUMP_SPEED_Y		0.375f
#define MARIO_JUMP_RUN_SPEED_Y	0.4f
#define TANUKI_FLAP_SPEED_Y		0.14f
#define TANUKI_JUMP_SPEED_Y		0.3f
#define TANUKI_JUMP_RUN_SPEED_Y	0.35f

#define MARIO_TIME_RUNTOFLY		1000
#define MARIO_TIME_FLYTOWALK	5000

#define MARIO_GRAVITY			0.00095f
#define TANUKI_GRAVITY			0.00075f

#define MARIO_JUMP_DEFLECT_SPEED  0.25f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT		400
#define MARIO_STATE_SHELLRUNNING_RIGHT	401
#define MARIO_STATE_RUNNING_LEFT		500
#define MARIO_STATE_SHELLRUNNING_LEFT	501


#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_KICK_RIGHT		700
#define MARIO_STATE_KICK_LEFT		701

#define MARIO_STATE_TRANSFORM_TANUKI	800

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 502
#define ID_ANI_MARIO_RUNNING_LEFT 503

#define ID_ANI_MARIO_RUNNING_RIGHT_MAXPOWER 600
#define ID_ANI_MARIO_RUNNING_LEFT_MAXPOWER 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_KICK_RIGHT 1021
#define ID_ANI_MARIO_KICK_LEFT 1020

#define ID_ANI_MARIO_RUNSHELLRIGHT	1050	
#define ID_ANI_MARIO_RUNSHELLLEFT	1051	

#define ID_ANI_MARIO_IDLESHELLRIGHT	1053	
#define ID_ANI_MARIO_IDLESHELLLEFT	1054	

#define ID_ANI_MARIO_STAND			1052

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1202
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1203

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT_MAXPOWER 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT_MAXPOWER 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1702
#define ID_ANI_MARIO_SMALL_KICK_LEFT 1703

#define ID_ANI_MARIO_SMALL_RUNSHELLRIGHT	1602	
#define ID_ANI_MARIO_SMALL_RUNSHELLLEFT		1603	

#define ID_ANI_MARIO_SMALL_IDLESHELLRIGHT	1604	
#define ID_ANI_MARIO_SMALL_IDLESHELLLEFT	1605

//TANUKI MARIO
#define ID_ANI_TANUKI_IDLE_RIGHT 1700
#define ID_ANI_TANUKI_IDLE_LEFT 1701

#define ID_ANI_TANUKI_WALKING_RIGHT 1800
#define ID_ANI_TANUKI_WALKING_LEFT 1801

#define ID_ANI_TANUKI_RUNNING_RIGHT 1802
#define ID_ANI_TANUKI_RUNNING_LEFT 1803

#define ID_ANI_TANUKI_RUNNING_RIGHT_MAXPOWER 1900
#define ID_ANI_TANUKI_RUNNING_LEFT_MAXPOWER 1901

#define ID_ANI_TANUKI_JUMP_WALK_RIGHT		2000
#define ID_ANI_TANUKI_JUMP_WALK_RIGHT_UP	2001
#define ID_ANI_TANUKI_JUMP_WALK_RIGHT_DOWN	2002
#define ID_ANI_TANUKI_JUMP_WALK_LEFT		2003
#define ID_ANI_TANUKI_JUMP_WALK_LEFT_UP		2004
#define ID_ANI_TANUKI_JUMP_WALK_LEFT_DOWN	2005

#define ID_ANI_TANUKI_JUMP_RUN_RIGHT_UP		2100
#define ID_ANI_TANUKI_JUMP_RUN_RIGHT_DOWN	2101
#define ID_ANI_TANUKI_JUMP_RUN_LEFT_UP		2102
#define ID_ANI_TANUKI_JUMP_RUN_LEFT_DOWN	2103

#define ID_ANI_TANUKI_SIT_RIGHT 2200
#define ID_ANI_TANUKI_SIT_LEFT 2201

#define ID_ANI_TANUKI_BRACE_RIGHT 2300
#define ID_ANI_TANUKI_BRACE_LEFT 2301

#define ID_ANI_TANUKI_KICK_RIGHT 2321
#define ID_ANI_TANUKI_KICK_LEFT 2320

#define ID_ANI_TANUKI_RUNSHELLRIGHT	2450	
#define ID_ANI_TANUKI_RUNSHELLLEFT	2451	

#define ID_ANI_TANUKI_IDLESHELLRIGHT	2452	
#define ID_ANI_TANUKI_IDLESHELLLEFT		2453	

#define ID_ANI_TRANSFORM_TANUKI			2511
#define ID_ANI_TRANSFORM_BIG_RIGHT		2512	
#define ID_ANI_TRANSFORM_BIG_LEFT		2513	
#define ID_ANI_TRANSFORM_SMALL_RIGHT	2514
#define ID_ANI_TRANSFORM_SMALL_LEFT		2515

#pragma endregion


#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TANUKI	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 26
#define MARIO_TANUKI_BBOX_HEIGHT 28
#define MARIO_TANUKI_BBOX_WIDTH	20
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 18

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SIT_HEIGHT_ADJUST_TANUKI ((MARIO_TANUKI_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_UNTOUCHABLE_TIME	2500
#define MARIO_KICK_DURATION		100


#define MARIO_SHELL_OFFSETX		11
#define MARIO_SHELL_OFFSETY		2

#define MARIO_SMALL_SHELL_OFFSETX		10
#define MARIO_SMALL_SHELL_OFFSETY		-1

#define TANUKI_SHELL_OFFSETX		14
#define TANUKI_SHELL_OFFSETY		2

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	bool isHolding = false;

	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	ULONGLONG kick_start;
	ULONGLONG transform_start = 0;
	ULONGLONG flap_start = 0; // Thời điểm bắt đầu vỗ cánh
	ULONGLONG running_start = 0;
	ULONGLONG running_maxpower_start = 0;
	ULONGLONG flying_start = 0;
	ULONGLONG floating_start = 0;

	BOOLEAN isOnPlatform;
	
	bool isTransforming = false;
	bool finishTransforming = false;
	
	bool isFlying = false; // Mario đang trong trạng thái bay
	bool isFlapping = false; // Mario vừa nhấn nút nhảy để vỗ cánh
	bool isFloating = false;
	bool maxPower = false;
	bool isSkidding; // Trạng thái trượt
	bool isTurning;  // Trạng thái quay đầu
	int transform_from = -1;
	int transform_to = -1;
	int coin; 

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithItemBox(LPCOLLISIONEVENT e);
	void OnCollisonWithGiant(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithDmgObject(LPCOLLISIONEVENT e);


	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTanuki();

public:
	static CMario* __instance;
	static CMario* GetInstance() { return __instance; }
	
	CKoopa* heldKoopa = nullptr;

	CMario(float x, float y) : CGameObject(x, y)
	{
		__instance = this; // Gán instance
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 
		LPGAMEOBJECT ea = new CEnemyActivator();
		ea->SetPosition(x + 100.0f, y);
		CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (scene)
		{
			scene->AddObject(ea);
		}
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		kick_start = -1;
		isOnPlatform = false;
		coin = 0;
		this->renderLayer = 10;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; };
	void GetAcc(float& ax, float& ay) { ax = this->ax; ay = this->ay; };
	void GetShellOffset(float& ox, float& oy) {
		switch (level)
		{
			case MARIO_LEVEL_SMALL:
			{
				ox = MARIO_SMALL_SHELL_OFFSETX;
				oy = MARIO_SMALL_SHELL_OFFSETY;
				break;
			}
			case MARIO_LEVEL_BIG:
			{
				ox = MARIO_SHELL_OFFSETX;
				oy = MARIO_SHELL_OFFSETY;
				break;
			}
			case MARIO_LEVEL_TANUKI:
			{
				ox = TANUKI_SHELL_OFFSETX;
				oy = TANUKI_SHELL_OFFSETY;
				break;
			}
		}
	}
	bool GetMaxPower() { return maxPower; };
	float GetAy() { return ay; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void StartFlap();
	void TakeDmg();
	bool IsMario() { return true; }
};