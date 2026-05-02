#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <map>  
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <random>
#include <iostream> 
#include <io.h>
#include <fstream>
#include <mmsystem.h>   
#include <mciapi.h> 

using namespace std;

#define ANGLETOX(p) (cosf(D3DXToRadian((p))))
#define ANGLETOY(p) (sinf(D3DXToRadian((p))))
#define GETANGLE(x1, y1, x2, y2) (D3DXToDegree(atan2f((y2 - y1), (x2 - x1))))

#include "Singleton.h"
#include "Time.h"
#include "FRECT.h"
#include "KeyManager.h"
#include "Random.h"

#include "Direct3D.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Object.h"
#include "Scene.h"
#include "Sound.h"

#include "Effect.h"
#include "Skill.h"
#include "Bullet.h"
#include "SplitBullet.h"
#include "Item.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "MapLayer.h"
#include "UI.h"
#include "Button.h"
#include "RankingManager.h"
#include "EnemySpawnObject.h"

#include "LoadingScene.h"
#include "TitleScene.h"
#include "HowToScene.h"
#include "RankingScene.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "ResultScene.h"