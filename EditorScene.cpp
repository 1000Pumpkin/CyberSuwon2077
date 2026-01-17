#include "pch.h"

void EditorScene::Save()
{
    ofstream os;
    os.open("stage.txt");
    
    vector<EnemySpawnObject*> spawnObjs;
    for (auto iter : objects)
    {
        EnemySpawnObject* so = dynamic_cast<EnemySpawnObject*>(iter);

        if (so)
            spawnObjs.push_back(so);
    }

    os << spawnDeltaTime << endl;
    os << spawnObjs.size() << endl;
    for (auto iter : spawnObjs)
        iter->Write(os);

    os.close();
}

void EditorScene::Load()
{
    for (auto iter : objects)
    {
        EnemySpawnObject* so = dynamic_cast<EnemySpawnObject*>(iter);

        if (so)
            so->isRemove = true;
    }

    ifstream is;
    is.open("stage.txt");

    spawnDeltaTime = 3.f;
    is >> loadSpawnDeltaTime;
    int spawnCount;
    is >> spawnCount;
    for (int i = 0; i < spawnCount; ++i)
    {
        spawnPlan.push_back(new EnemySpawnObject(is));
    }

    is.close();

    Time::GetInstance()->timeScale = 1.f;
    Time::GetInstance()->prevTime = Time::GetInstance()->curTime - DWORD(loadSpawnDeltaTime * 1000.f);
}

void EditorScene::NextCursor()
{
    if (++cursorType >= EnemyType::Count)
        cursorType = 0;

    cursorSprite = Sprite("Assets/enemy/" + Enemy::enemyTypeStrs[cursorType] + "/*.png");
}

void EditorScene::Init()
{
    NextCursor();

    PlayerManager::GetInstance()->Reset();
    PlayerManager::GetInstance()->g_pPlayer = new Player;
    AddObject(PlayerManager::GetInstance()->g_pPlayer);

    spawnDeltaTime = 3.f;
    Direct3D::GetInstance()->zoom = 2.f;
    Time::GetInstance()->timeScale = 0.f;
}

void EditorScene::Term()
{
}

void EditorScene::Update(float deltaTime)
{
    spawnDeltaTime += deltaTime;

    for (auto iter = spawnPlan.begin(); iter != spawnPlan.end();)
    {
        if ((*iter)->info.spawnTime >= spawnDeltaTime)
        {
            AddObject((*iter));
            iter = spawnPlan.erase(iter);
        }
        else
            ++iter;
    }

    if (KeyManager::GetInstance()->GetKeyState('1') == EKEY_PRESS)
    {
        NextCursor();
    }

    if (spawnDeltaTime >= loadSpawnDeltaTime)
    {
        if (KeyManager::GetInstance()->GetKeyState('2') == EKEY_DOWN)
        {
            Time::GetInstance()->timeScale = 0.5f;
        }
        else
        {
            Time::GetInstance()->timeScale = 0.f;

            if (KeyManager::GetInstance()->GetKeyState('3') == EKEY_DOWN)
            {
                Time::GetInstance()->timeScale = -0.5f;
            }
            else
            {
                Time::GetInstance()->timeScale = 0.f;
            }
        }
    }

    if (KeyManager::GetInstance()->GetKeyState('4') == EKEY_PRESS)
    {
        Save();
    }

    if (KeyManager::GetInstance()->GetKeyState('5') == EKEY_PRESS)
    {
        Load();
    }

    if (KeyManager::GetInstance()->GetKeyState('6') == EKEY_PRESS)
    {
        EnemySpawnInfo esInfo;
        esInfo.pathes = lastSpawn->info.pathes;
        esInfo.rotate = lastSpawn->info.rotate;
        esInfo.spawnTime = spawnDeltaTime;
        esInfo.type = (EnemyType)cursorType;

        SceneManager::GetInstance()->AddObject(new EnemySpawnObject(esInfo));
    }

    if (KeyManager::GetInstance()->GetKeyState('7') == EKEY_PRESS)
    {
        EnemySpawnInfo esInfo;
        esInfo.pathes = lastSpawn->info.pathes;
        for (auto& iter : esInfo.pathes)
        {
            iter.pos.y = 360 + (360 - iter.pos.y);
        }
        esInfo.rotate = 360 - lastSpawn->info.rotate;
        esInfo.spawnTime = spawnDeltaTime;
        esInfo.type = (EnemyType)cursorType;

        SceneManager::GetInstance()->AddObject(new EnemySpawnObject(esInfo));
    }

    if (KeyManager::GetInstance()->GetKeyState('0') == EKEY_PRESS)
    {
        for (auto iter = objects.rbegin(); iter != objects.rend(); ++iter)
        {
            if ((*iter)->tag == "SpawnObject")
            {
                (*iter)->isRemove = true;
                break;
            }
        }
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_LBUTTON) == EKEY_PRESS)
    {
        EnemyMovePath path;
        path.isAttack = false;
        path.pos = cursorSprite.pos;
        pathes.push_back(path);
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_MBUTTON) == EKEY_PRESS)
    {
        EnemyMovePath path;
        path.isAttack = true;
        path.pos = cursorSprite.pos;
        pathes.push_back(path);
    }

    if (KeyManager::GetInstance()->GetKeyState(VK_RBUTTON) == EKEY_PRESS)
    {
        if (pathes.size() > 0)
        {
            EnemySpawnInfo esInfo;
            esInfo.pathes = pathes;
            esInfo.rotate = cursorSprite.rotate;
            esInfo.spawnTime = spawnDeltaTime;
            esInfo.type = (EnemyType)cursorType;
            lastSpawn = new EnemySpawnObject(esInfo);

            SceneManager::GetInstance()->AddObject(lastSpawn);

            pathes.clear();
        }
    }

    if (KeyManager::GetInstance()->GetKeyState('Q') == EKEY_PRESS)
    {
        cursorSprite.rotate += 15;
    }
    if (KeyManager::GetInstance()->GetKeyState('E') == EKEY_PRESS)
    {
        cursorSprite.rotate -= 15;
    }
    Scene::Update(deltaTime);
}

void EditorScene::Render()
{
    cursorSprite.pos = KeyManager::GetInstance()->GetCursorPosition() + Direct3D::GetInstance()->camPos;
    cursorSprite.Render();

    Scene::Render();

    D3DXVECTOR3 square[5] = {
        D3DXVECTOR3(0, 0, 0),
        D3DXVECTOR3(SCREEN_WIDTH, 0, 0),
        D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
        D3DXVECTOR3(0, SCREEN_HEIGHT, 0),
        D3DXVECTOR3(0, 0, 0)
    };

    auto line = Direct3D::GetInstance()->pLine;

    D3DXMATRIX lineMat = Direct3D::GetInstance()->viewMat * Direct3D::GetInstance()->projMat;
    line->Begin();
    line->DrawTransform(square, 5, &lineMat, 0xffff0000);
    line->End();

    RECT rc = { 0, 0, 200, 200 };
    char buffer[255];
    sprintf(buffer, "%.2f", spawnDeltaTime);
    Direct3D::GetInstance()->pFont->DrawTextA(NULL, buffer, -1, &rc, DT_CENTER | DT_VCENTER, 0xffffffff);
}
