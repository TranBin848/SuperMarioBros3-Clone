#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "BgObject.h"
#include "Brick.h"
#include "ItemBox.h"
#include "Giant.h"
#include "Leaf.h"
#include "VenusFire.h"
#include "GreenVenusFire.h"
#include "PiranhaTrap.h"
#include "Koopa.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "Obstacle.h"
#include "Pipe.h"
#include "Card.h"
#include "Mario.h"
#include "SampleKeyEventHandler.h"

#define CAM_MAX_Y	3.0f
#define CAM_MIN_Y	-30.0f

using namespace std;

CPlayScene::CPlayScene(int id, const wstring& filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 3) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		if (CGame::GetInstance()->GetIsExitingPipe() == true)
		{
			obj = new CMario(2500, 110);
			// General object setup
			obj->SetPosition(2374,162);
			objects.push_back(obj);
			player = (CMario*)obj;
			return;
		}
		else
		{
			obj = new CMario(x, y);
			player = (CMario*)obj;
		}

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_PARAGOOMBA: obj = new CParaGoomba(x, y); break;
	case OBJECT_TYPE_KOOPA:
	{
		int flag = (int)atof(tokens[3].c_str());
		obj = new CKoopa(x, y, flag); break;
	}
	case OBJECT_TYPE_OBSTACLE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int sprite_id = atoi(tokens[5].c_str());
		obj = new CObstacle(x, y, cell_width, cell_height, sprite_id); 
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int sprite_id = atoi(tokens[5].c_str());
		int fl = (int)atof(tokens[6].c_str());
		obj = new CPipe(x, y, cell_width, cell_height, sprite_id, fl);
		break;
	}
	case OBJECT_TYPE_SHINYBRICK:
	{
		int flag = atoi(tokens[3].c_str());
		obj = new CBrick(
			x, y, flag);
		break;
	}
	
	case OBJECT_TYPE_COIN:
	{
		int flag = atoi(tokens[3].c_str());
		obj = new CCoin(x, y, flag); break; 
	}
	case OBJECT_TYPE_CARD:
	{
		obj = new CCard(x, y); break;
	}
	case OBJECT_TYPE_VENUSFIRE: obj = new CVenusFire(x,y); break;
	case OBJECT_TYPE_GREENVENUSFIRE: obj = new CGreenVenusFire(x, y); break;
	case OBJECT_TYPE_PIRANHATRAP: obj = new CPiranhaTrap(x, y); break;
	case OBJECT_TYPE_ITEMBOX: {
		int flag = atoi(tokens[3].c_str());
		obj = new CItemBox(x, y, flag); break;
	}
	case OBJECT_TYPE_BGOBJECT:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		obj = new CBgObject(x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end);
		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());


		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, false
		);

		break;
	}
	case OBJECT_TYPE_FLOOR:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, true
		);

		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOutTitle(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	CGame* game = CGame::GetInstance();
	CHUD::GetInstance()->SetPosition(game->GetBackBufferWidth() / 2, game->GetBackBufferHeight() + 1.0f);
	
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	CHUD::GetInstance()->Update(dt, &objects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	// Camera follow theo X
	float cx, cy;
	float marioX, marioY;
	player->GetPosition(marioX, marioY);

	cx = marioX - game->GetBackBufferWidth() / 2;
	cy = marioY - game->GetBackBufferHeight() / 2;
	
	// Giới hạn camera theo trục x
	if (cx < 0) cx = 0;
	if (cx > 2562.0f) cx = 2562.0f;
	// Giới hạn camera theo trục y
	int currentScene = CGame::GetInstance()->GetCurrentSceneId();
	if (currentScene == 1)
	{
		cy = CAM_MAX_Y; // Giữ y cố định ở 0 cho scene 1
	}
	else
	{
		if (cy > CAM_MAX_Y) cy = CAM_MAX_Y;
		else if ((CAM_MIN_Y < cy) && (cy < CAM_MAX_Y)) cy = CAM_MAX_Y;
		else
		{
			if (CMario::GetInstance()->GetLevel() == MARIO_LEVEL_TANUKI)
			{
				if (CMario::GetInstance()->GetMaxPower())
					cy = CAM_MAX_Y + cy - CAM_MIN_Y;
				else
					if (cy < CAM_MIN_Y)
						cy = CAM_MAX_Y + cy - CAM_MIN_Y;
					else
						cy = CAM_MAX_Y;
			}
			else
			{
				cy = CAM_MAX_Y;
			}
		}
	}
	/*DebugOutTitle(L"camX: %f", cx);*/
	/*DebugOutTitle(L"y: %f, Mario y: %f", cy, marioY);*/
	CGame::GetInstance()->SetCamPos(cx, cy);
	
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	// Tạo bản sao vector object để không làm thay đổi thứ tự update
	std::vector<LPGAMEOBJECT> sortedObjects = objects;

	// Sắp xếp theo renderLayer tăng dần (vẽ sau thì đè lên trước)
	std::sort(sortedObjects.begin(), sortedObjects.end(),
		[](LPGAMEOBJECT a, LPGAMEOBJECT b) {
			return a->GetRenderLayer() < b->GetRenderLayer();
		});

	for (int i = 0; i < sortedObjects.size(); i++)
		sortedObjects[i]->Render();

	CHUD::GetInstance()->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	CAnimations::GetInstance()->Clear(); // POTENTIALLY DANGEROUS if global
	CSprites::GetInstance()->Clear();    // POTENTIALLY DANGEROUS if global
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::AddObject(LPGAMEOBJECT obj)
{
	if (obj != nullptr)
	{
		objects.push_back(obj);
	}
}
void CPlayScene::InsertObjectBefore(LPGAMEOBJECT target, LPGAMEOBJECT newObj)
{
	auto it = std::find(objects.begin(), objects.end(), target);
	if (it != objects.end())
	{
		objects.insert(it, newObj); // chèn trước target
	}
	else
	{
		objects.push_back(newObj); // fallback: nếu không tìm thấy thì thêm vào cuối
	}
}