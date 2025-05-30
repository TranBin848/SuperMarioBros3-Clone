﻿#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "HUD.h" 


#define CAM_MAX_Y	3.0f
#define CAM_MIN_Y	-40.0f

#define EFFECT_PLAY_TIMEOUT 1000
#define PLAYER_DIE_TIMEOUT 2000

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					
	ULONGLONG player_die_start = -1;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, const wstring& filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	void AddObject(LPGAMEOBJECT obj);
	void InsertObjectBefore(LPGAMEOBJECT target, LPGAMEOBJECT newObj);
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

